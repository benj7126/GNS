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
	// std::cout << "test char pos thingy: " << GetCursorIndex(pos) << std::endl;
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
				Vector2 res = MeasureTextWithChar((char)key);
				if (res.x <= size.x && res.y <= size.y) {
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
			Vector2 res = MeasureTextWithChar('\n');
			if (res.x <= size.x && res.y <= size.y) {
				text.insert(text.begin()+cursorPosition, '\n');
				cursorPosition++;
			}
		}

		if (IsKeyPressed(KEY_LEFT))
			cursorPosition = cursorPosition == 0 ? 0 : cursorPosition-1;

		if (IsKeyPressed(KEY_RIGHT))
			cursorPosition = cursorPosition == text.size() ? text.size() : cursorPosition+1;
	}
	
	std::cout << MeasureTextWithChar('a').x << " - " << MeasureTextWithChar('a').y << std::endl;
}


// need to make it so that only one element can be selected at a time and then make it work that way around instead of making TextInputHandler edit a pointer...

void Elements::TextField::DrawCodepointAt(Vector2 origin, std::vector<int> codepointBuffer, int& curIndex, float &textOffsetX, float textOffsetY){
	Font font = FontManager::GetFont(fontType, fontSize);

	if (codepointBuffer.size() > 0){
		for (int cp : codepointBuffer){
			DrawTextCodepoint(font, cp, { origin.x + textOffsetX, origin.y + textOffsetY }, fontSize, BLACK);
			
			std::cout << curIndex << " is " << (curIndex == cursorPosition) << std::endl;

			if (curIndex == cursorPosition)
				DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);

			curIndex++; // idk if this should be here or before, but whatever...

			int index = GetGlyphIndex(font, cp);
			if (font.glyphs[index].advanceX == 0) textOffsetX += ((float)font.recs[index].width + spacing);
			else textOffsetX += ((float)font.glyphs[index].advanceX + spacing);
		}
	}
}

void Elements::TextField::CustomTextDraw(Vector2 origin) {
	Font font = FontManager::GetFont(fontType, fontSize);
	const char* cText = text.c_str();
	
	// basically just raylibs code, lol
    int size = TextLength(cText);    // Total size in bytes of the text, scanned by codepoints in loop

	std::vector<int> codepointBuffer{};
	float codepointBufferWidth = 0;
	float curLineWidth = 0;

	int curIndex = 0;

    float textOffsetY = 0;            // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    for (int i = 0; i < size;) {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);

        if (codepoint == '\n')
        {
			DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY);

			codepointBuffer.clear();
			codepointBufferWidth = 0;

            // NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup
            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
			curLineWidth = 0;
        }
        else
        {
			if (codepoint == ' ' || codepoint == '\t' || !wordWrap) {
				DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY);

				codepointBuffer.clear();
				curLineWidth += codepointBufferWidth;
				codepointBufferWidth = 0;

				if (codepoint == ' ' || codepoint == '\t') { // if its a space still add the spaces worht to size
					if (curIndex == cursorPosition)
						DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);

					curIndex++;

					int index = GetGlyphIndex(font, codepoint);
					if (font.glyphs[index].advanceX == 0){
						textOffsetX += ((float)font.recs[index].width + spacing);
						curLineWidth += ((float)font.recs[index].width + spacing);
					} else {
						textOffsetX += ((float)font.glyphs[index].advanceX + spacing);
						curLineWidth += ((float)font.glyphs[index].advanceX + spacing);
					}
				}
            }

            if ((codepoint != ' ') && (codepoint != '\t')) {
				codepointBuffer.push_back(codepoint);

				int index = GetGlyphIndex(font, codepoint);
				if (font.glyphs[index].advanceX == 0) codepointBufferWidth += ((float)font.recs[index].width + spacing);
				else codepointBufferWidth += ((float)font.glyphs[index].advanceX + spacing);
            }
			
			if (curLineWidth + codepointBufferWidth >= this->size.x) {
				int savedCodepoint = -1;

				if (curLineWidth == 0){ // clear buffer if theres nothing else in this line
					if (curLineWidth + codepointBufferWidth > this->size.x){
						savedCodepoint = codepointBuffer.back();
						codepointBuffer.pop_back();
					}

					DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY);

					codepointBuffer.clear();
					codepointBufferWidth = 0;
				}

				if (i != size - 1){
					// act like a newline
					textOffsetY += textLineSpacing;
					textOffsetX = 0.0f;
					curLineWidth = 0;
				}

				if (savedCodepoint != -1){
					codepointBuffer.push_back(savedCodepoint);

					int index = GetGlyphIndex(font, savedCodepoint);
					if (font.glyphs[index].advanceX == 0) codepointBufferWidth += ((float)font.recs[index].width + spacing);
					else codepointBufferWidth += ((float)font.glyphs[index].advanceX + spacing);
				}
			}
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }

	DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY);

	if (curIndex == cursorPosition)
		DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);

	std::cout << " f: " << curLineWidth + codepointBufferWidth << " - " << this->size.x << std::endl;
}

int Elements::TextField::GetCursorIndex(Vector2 localMousePosition) {
	Font font = FontManager::GetFont(fontType, fontSize);
	const char* cText = text.c_str();

	// basically just raylibs code, lol
    int size = TextLength(cText);    // Total size in bytes of the text, scanned by codepoints in loop

	int curIndex = 0;

    int textOffsetY = 0;            // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw
    for (int i = 0; i < size;)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&cText[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        if (codepoint == '\n')
        {
            // NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup
            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
        }
        else
        {
			float xToAdd = 0;

            if (font.glyphs[index].advanceX == 0) xToAdd = ((float)font.recs[index].width + spacing);
            else xToAdd = ((float)font.glyphs[index].advanceX + spacing);

            if ((codepoint != ' ') && (codepoint != '\t'))
            {
				Vector2 charPosition = { textOffsetX, textOffsetY };
				if (charPosition.x + xToAdd / 2 > localMousePosition.x && charPosition.y < localMousePosition.y && charPosition.y + fontSize > localMousePosition.y)
					return curIndex;

				curIndex++; // idk if this should be here or before, but whatever...
            }

			textOffsetX += xToAdd;
			
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }

	return text.size(); // or -1 idk yet
}

Vector2 Elements::TextField::MeasureTextWithChar(char extraChar){
	Font font = FontManager::GetFont(fontType, fontSize);
	const char* cText = text.c_str();
	
	// basically just raylibs code, lol
    int size = TextLength(cText);    // Total size in bytes of the text, scanned by codepoints in loop

	Vector2 textSize{0};

	std::vector<int> codepointBuffer{};
	float codepointBufferWidth = 0;
	float curLineWidth = 0;

	int curIndex = 0;

    float textOffsetY = 0;            // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    for (int i = 0; i < size;) {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
		float lastCodepointBufferWidth = codepointBufferWidth;

        if (codepoint == '\n')
        {
			// DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY);
			
			if (curLineWidth + lastCodepointBufferWidth > textSize.x)
				textSize.x = curLineWidth + lastCodepointBufferWidth;

			codepointBuffer.clear();
			codepointBufferWidth = 0;

            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
			curLineWidth = 0;
        }
        else
        {
			if (codepoint == ' ' || codepoint == '\t' || !wordWrap) {
				// DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY);

				codepointBuffer.clear();
				curLineWidth += codepointBufferWidth;
				codepointBufferWidth = 0;

				if (codepoint == ' ' || codepoint == '\t') { // if its a space still add the spaces worht to size
					// if (curIndex == cursorPosition)
					// 	DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);

					curIndex++;

					int index = GetGlyphIndex(font, codepoint);
					if (font.glyphs[index].advanceX == 0){
						textOffsetX += ((float)font.recs[index].width + spacing);
						curLineWidth += ((float)font.recs[index].width + spacing);
					} else {
						textOffsetX += ((float)font.glyphs[index].advanceX + spacing);
						curLineWidth += ((float)font.glyphs[index].advanceX + spacing);
					}
				}
            }

            if ((codepoint != ' ') && (codepoint != '\t')) {
				codepointBuffer.push_back(codepoint);

				int index = GetGlyphIndex(font, codepoint);
				if (font.glyphs[index].advanceX == 0) codepointBufferWidth += ((float)font.recs[index].width + spacing);
				else codepointBufferWidth += ((float)font.glyphs[index].advanceX + spacing);
            }
			
			if (curLineWidth + codepointBufferWidth >= this->size.x) {
				int savedCodepoint = -1;

				if (curLineWidth == 0){ // clear buffer if theres nothing else in this line
					if (curLineWidth + codepointBufferWidth > this->size.x){
						savedCodepoint = codepointBuffer.back();
						codepointBuffer.pop_back();
					} else {
					if (curLineWidth + codepointBufferWidth > textSize.x)
						textSize.x = curLineWidth + codepointBufferWidth;
					}

					// DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY);

					codepointBuffer.clear();
					codepointBufferWidth = 0;
				}
				
				if (curLineWidth + lastCodepointBufferWidth > textSize.x)
					textSize.x = curLineWidth + lastCodepointBufferWidth;

				// act like a newline
				textOffsetY += textLineSpacing;
				textOffsetX = 0.0f;
				curLineWidth = 0;

				if (savedCodepoint != -1){
					codepointBuffer.push_back(savedCodepoint);

					int index = GetGlyphIndex(font, savedCodepoint);
					if (font.glyphs[index].advanceX == 0) codepointBufferWidth += ((float)font.recs[index].width + spacing);
					else codepointBufferWidth += ((float)font.glyphs[index].advanceX + spacing);
				}
			}
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }

	// DrawCodepointAt(origin, codepointBuffer, curIndex, textOffsetX, textOffsetY);

	// if (curIndex == cursorPosition)
	// 	DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);

	// std::cout << " f: " << curLineWidth + codepointBufferWidth << " - " << this->size.x << std::endl;

	if (curLineWidth + codepointBufferWidth > textSize.x)
		textSize.x = curLineWidth + codepointBufferWidth;
	
	textSize.y = textOffsetY + fontSize;

	return textSize;
}