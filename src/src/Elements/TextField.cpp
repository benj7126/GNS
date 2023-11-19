#include "Elements/TextField.h"

#include "FontManager.h"

#include <iostream>
#include <vector>

void Elements::TextField::InternalDraw(Vector2 offset) {
	// std::cout << offset.x + position.x << offset.y + position.y << size.x << size.y << std::endl;
	// std::cout << textFont << std::endl;
	
	Font font = FontManager::GetFont(fontType, fontSize);
	
	// size.y = fontSize; // might want to set this when you change size instead, as to make it constant somehow...
	DrawRectangle(offset.x + position.x, offset.y + position.y, size.x, size.y, ORANGE); // size is {0, 0}...
	
	CustomTextDraw({offset.x + position.x, offset.y + position.y});
}

void Elements::TextField::MousePressed(Vector2 pos) {
	std::cout << "test char pos thingy: " << GetCursorIndex(pos) << std::endl;
	cursorPosition = GetCursorIndex(pos);
	selectedElement = this;
	// TextInputHandler::LinkForInput(&text);
}

void Elements::TextField::Update(){
	if (selectedElement == this){
		int key = GetCharPressed();

		while (key > 0)
		{
			if ((key >= 32) && (key <= 125))
			{
				float nHeight = MeasureTextHeightWithChar((char)key);
				if (nHeight <= size.y) {
					text.insert(text.begin()+cursorPosition, (char)key);
					cursorPosition++;
				}
			}

			key = GetCharPressed();
		}

		if (IsKeyPressed(KEY_BACKSPACE))
		{
			if (cursorPosition != 0){
				text.erase(text.begin() + cursorPosition-1);
				cursorPosition--;
			}
		}

		if (IsKeyPressed(KEY_DELETE))
		{
			if (cursorPosition != text.size()){
				text.erase(text.begin() + cursorPosition);
				cursorPosition;
			}
		}

		if (IsKeyPressed(KEY_ENTER))
		{
			float nHeight = MeasureTextHeightWithChar('\n');
			if (nHeight <= size.y) {
				text.insert(text.begin()+cursorPosition, '\n');
				cursorPosition++;
			}
		}

		if (IsKeyPressed(KEY_LEFT))
			cursorPosition = cursorPosition == 0 ? 0 : cursorPosition-1;

		if (IsKeyPressed(KEY_RIGHT))
			cursorPosition = cursorPosition == text.size() ? text.size() : cursorPosition+1;
	}
	
	// std::cout << MeasureTextWithChar('a').x << " - " << MeasureTextWithChar('a').y << std::endl;
}


// need to make it so that only one element can be selected at a time and then make it work that way around instead of making TextInputHandler edit a pointer...

void Elements::TextField::DrawCodepointAt(Vector2 origin, std::vector<int> codepointBuffer, int& curIndex, float &textOffsetX, float textOffsetY, bool &didDrawCursor){
	Font font = FontManager::GetFont(fontType, fontSize);

	if (codepointBuffer.size() > 0){
		for (int cp : codepointBuffer){
			DrawTextCodepoint(font, cp, { origin.x + textOffsetX, origin.y + textOffsetY }, fontSize, BLACK);
			
			// std::cout << curIndex << " is " << (curIndex == cursorPosition) << std::endl;

			if (curIndex == cursorPosition && !didDrawCursor){
				didDrawCursor = true;
				DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);
			}

			curIndex++; // idk if this should be here or before, but whatever...

			int index = GetGlyphIndex(font, cp);
			textOffsetX += (font.glyphs[index].advanceX == 0 ? (float)font.recs[index].width : (float)font.glyphs[index].advanceX) + spacing;
		}
	}
}

void Elements::TextField::CustomTextDraw(Vector2 origin) {
	Font font = FontManager::GetFont(fontType, fontSize);
	const char* cText = text.c_str();

	bool didDrawCursor = false;
	if (selectedElement != this)
		didDrawCursor = true;
	
    int size = TextLength(cText);    // Total size in bytes of the text, scanned by codepoints in loop

	std::vector<int> codepointBuffer{};
	float codepointBufferWidth = 0;
	float curLineWidth = 0;

	float nextCharWidth = 0;

	int curIndex = 0;

    float textOffsetY = 0;            // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    for (int i = 0; i < size;) {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
		int cpIndex = GetGlyphIndex(font, codepoint);
		
		if (i+1 < size){
			int nextCodepointIndex = GetGlyphIndex(font, codepoint);
			nextCharWidth = (font.glyphs[nextCodepointIndex].advanceX == 0 ? (float)font.recs[nextCodepointIndex].width : (float)font.glyphs[nextCodepointIndex].advanceX) + spacing;
		} else {
			nextCharWidth = 0;
		}

        if (codepoint == '\n')
        {
			DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY, didDrawCursor);

			codepointBuffer.clear();
			codepointBufferWidth = 0;

            // NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup
			if (curIndex == cursorPosition && !didDrawCursor){
				didDrawCursor = true;
				DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);
			}
			curIndex++;

            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
			curLineWidth = 0;
        }
        else
        {
			codepointBuffer.push_back(codepoint);
			codepointBufferWidth += (font.glyphs[cpIndex].advanceX == 0 ? (float)font.recs[cpIndex].width : (float)font.glyphs[cpIndex].advanceX) + spacing;

			if (codepoint == ' ' || codepoint == '\t' || wrapping == 0 || wrapping == 2 || i + 1 == size || curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x) {
				if (curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x && wrapping != 2){
					bool wasZero = curLineWidth == 0;
					if ((wrapping == 1 && curLineWidth != 0) && textOffsetY + textLineSpacing + fontSize <= this->size.y) {
						textOffsetY += textLineSpacing;
						textOffsetX = 0.0f;
						curLineWidth = 0;
					}

					DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY, didDrawCursor);

					codepointBuffer.clear();
					curLineWidth += codepointBufferWidth;
					codepointBufferWidth = 0;

					if (wasZero || wrapping != 1) {
						if (!charWrapNextLine){
							if (curIndex == cursorPosition && !didDrawCursor){
								didDrawCursor = true;
								DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);
							}
						}
						
						textOffsetY += textLineSpacing;
						textOffsetX = 0.0f;
						curLineWidth = 0;
					}
				} else {
					DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY, didDrawCursor);

					codepointBuffer.clear();
					curLineWidth += codepointBufferWidth;
					codepointBufferWidth = 0;
				}
            }
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }

	if (!didDrawCursor)
		DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);

	// std::cout << " f: " << curLineWidth + codepointBufferWidth << " - " << this->size.x << std::endl;
}

float Elements::TextField::MeasureTextHeightWithChar(char extraChar){
	Font font = FontManager::GetFont(fontType, fontSize);
	const char* cText = text.c_str();
	
    int size = TextLength(cText) + 1;    // Total size in bytes of the text, scanned by codepoints in loop

	std::vector<int> codepointBuffer{};
	float codepointBufferWidth = 0;
	float curLineWidth = 0;

	float nextCharWidth = 0;

	int curIndex = 0;

    float textOffsetY = 0;            // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    for (int i = 0; i < size;) {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = i+1 == size ? GetCodepointNext(&extraChar, &codepointByteCount) : GetCodepointNext(&text[i], &codepointByteCount);
		int cpIndex = GetGlyphIndex(font, codepoint);
		
		if (i+1 < size){
			int nextCodepointIndex = GetGlyphIndex(font, codepoint);
			nextCharWidth = (font.glyphs[nextCodepointIndex].advanceX == 0 ? (float)font.recs[nextCodepointIndex].width : (float)font.glyphs[nextCodepointIndex].advanceX) + spacing;
		} else {
			nextCharWidth = 0;
		}

        if (codepoint == '\n')
        {
			codepointBuffer.clear();
			codepointBufferWidth = 0;

            // NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup
            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
			curLineWidth = 0;
        }
        else
        {
			codepointBuffer.push_back(codepoint);
			codepointBufferWidth += (font.glyphs[cpIndex].advanceX == 0 ? (float)font.recs[cpIndex].width : (float)font.glyphs[cpIndex].advanceX) + spacing;

			if (codepoint == ' ' || codepoint == '\t' || wrapping == 0 || wrapping == 2 || i + 1 == size || curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x) {
				if (curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x && wrapping != 2){
					bool wasZero = curLineWidth == 0;
					if ((wrapping == 1 && curLineWidth != 0) && textOffsetY + textLineSpacing + fontSize <= this->size.y) {
						textOffsetY += textLineSpacing;
						textOffsetX = 0.0f;
						curLineWidth = 0;
					}

					codepointBuffer.clear();
					curLineWidth += codepointBufferWidth;
					codepointBufferWidth = 0;

					if (wasZero || wrapping != 1) {
						textOffsetY += textLineSpacing;
						textOffsetX = 0.0f;
						curLineWidth = 0;
					}
				} else {
					codepointBuffer.clear();
					curLineWidth += codepointBufferWidth;
					codepointBufferWidth = 0;
				}
            }
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }

	return textOffsetY + fontSize;
}

int Elements::TextField::ScanCodepointsAt(Vector2 localMousePosition, std::vector<int> codepointBuffer, int& curIndex, float &textOffsetX, float textOffsetY){
	Font font = FontManager::GetFont(fontType, fontSize);

	if (codepointBuffer.size() > 0){
		for (int cp : codepointBuffer){
			int index = GetGlyphIndex(font, cp);
			float codepointWidth = (font.glyphs[index].advanceX == 0 ? (float)font.recs[index].width : (float)font.glyphs[index].advanceX) + spacing;

			if (textOffsetX - codepointWidth/2 <= localMousePosition.x && textOffsetX - codepointWidth/2 + codepointWidth >= localMousePosition.x && textOffsetY <= localMousePosition.y && textOffsetY + fontSize >= localMousePosition.y){
				return curIndex;
			}

			curIndex++;

			textOffsetX += codepointWidth;
		}
	}

	return -1;
}

int Elements::TextField::GetCursorIndex(Vector2 localMousePosition) {
	Font font = FontManager::GetFont(fontType, fontSize);
	const char* cText = text.c_str();
	
    int size = TextLength(cText);    // Total size in bytes of the text, scanned by codepoints in loop

	std::vector<int> codepointBuffer{};
	float codepointBufferWidth = 0;
	float curLineWidth = 0;

	float nextCharWidth = 0;

	int curIndex = 0;

    float textOffsetY = 0;            // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    for (int i = 0; i < size;) {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
		int cpIndex = GetGlyphIndex(font, codepoint);
		
		if (i+1 < size){
			int nextCodepointIndex = GetGlyphIndex(font, codepoint);
			nextCharWidth = (font.glyphs[nextCodepointIndex].advanceX == 0 ? (float)font.recs[nextCodepointIndex].width : (float)font.glyphs[nextCodepointIndex].advanceX) + spacing;
		} else {
			nextCharWidth = 0;
		}

        if (codepoint == '\n')
        {
			int index = ScanCodepointsAt(localMousePosition, codepointBuffer, curIndex, textOffsetX, textOffsetY);
			if (index != -1) {
				return index;
			}

			codepointBuffer.clear();
			codepointBufferWidth = 0;

            // NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup

			if (textOffsetY < localMousePosition.y && textOffsetY + fontSize > localMousePosition.y){
				std::cout << textOffsetY << " | " << localMousePosition.y << " - " << fontSize << std::endl;
				return curIndex;
			}

			curIndex++;

            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
			curLineWidth = 0;
        }
        else
        {
			codepointBuffer.push_back(codepoint);
			codepointBufferWidth += (font.glyphs[cpIndex].advanceX == 0 ? (float)font.recs[cpIndex].width : (float)font.glyphs[cpIndex].advanceX) + spacing;

			if (codepoint == ' ' || codepoint == '\t' || wrapping == 0 || wrapping == 2 || i + 1 == size || curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x) {
				if (curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x && wrapping != 2){
					bool wasZero = curLineWidth == 0;
					if ((wrapping == 1 && curLineWidth != 0) && textOffsetY + textLineSpacing + fontSize <= this->size.y) { // pressed at wrapped line
						if (textOffsetY < localMousePosition.y && textOffsetY + fontSize > localMousePosition.y){
							std::cout << textOffsetY << " | " << localMousePosition.y << " - " << fontSize << std::endl;
							return curIndex-1;
						}

						textOffsetY += textLineSpacing;
						textOffsetX = 0.0f;
						curLineWidth = 0;
					}

					int index = ScanCodepointsAt(localMousePosition, codepointBuffer, curIndex, textOffsetX, textOffsetY);
					if (index != -1) {
						return index;
					}

					codepointBuffer.clear();
					curLineWidth += codepointBufferWidth;
					codepointBufferWidth = 0;

					if (wasZero || wrapping != 1) { // pressed at wrapped line
						if (textOffsetY < localMousePosition.y && textOffsetY + fontSize > localMousePosition.y){
							std::cout << textOffsetY << " | " << localMousePosition.y << " - " << fontSize << std::endl;
							return curIndex;
						}

						textOffsetY += textLineSpacing;
						textOffsetX = 0.0f;
						curLineWidth = 0;
					}
				} else {
					int index = ScanCodepointsAt(localMousePosition, codepointBuffer, curIndex, textOffsetX, textOffsetY);
					if (index != -1) {
						return index;
					}

					codepointBuffer.clear();
					curLineWidth += codepointBufferWidth;
					codepointBufferWidth = 0;
				}
            }
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }


	return text.size(); // or -1 idk yet
}