#include "Elements/TextField.h"

#include "FontManager.h"

#include <algorithm>

#include <iostream>
#include <vector>

#include "KeyInput.h"

DataStructBase Elements::TextField::GenerateDataStruct() {
	DataStructHolder returnBase{};

	// returnBase.subStruct.insert();
	
	returnBase.insert("Label", Elements::Interfaces::ILabel::GetDataStruct());

	return returnBase;
}

void Elements::TextField::InternalDraw(Vector2 offset) {
	savedOffset = offset;
	DrawRectangle(offset.x + position.x, offset.y + position.y, size.x, size.y, ORANGE); // size is {0, 0}...
	CustomTextDraw({offset.x + position.x, offset.y + position.y});
}

void Elements::TextField::MousePressed(Vector2 pos) {
	// std::cout << "test char pos thingy: " << pos.x << ", " << pos.y << std::endl;
	int oldCP = cursorPosition;
	cursorPosition = GetCursorIndex(pos);
	// std::cout << GetTime() << std::endl;
	selectedElement = this;
	hasBeenReleased = false;
	savedX = -1;

	if (oldCP == cursorPosition){
		if (clickCount == 0)
			clickTime = GetTime();
		else if (GetTime() - clickTime <= 1)
			{ clickCount++; clickTime = GetTime(); }
		else
			clickCount = 0;

		if (clickCount > 0){
			std::vector<char> stopChars;
			
			switch (clickCount){
				case 1:
					stopChars = {' ', ',', '.'}; // might want to make character groups, where one group is space characters like tab and space and another is just , and ofc one thats letters(+nrs) and so on...
					break;
				case 2:
					stopChars = {'\n'};
					break;
				default:
					stopChars = {' ', ',', '.', '\n'};
					break;
			}

			int leftPosition = cursorPosition;
			int rightPosition = cursorPosition;

			char curChar = text.at(cursorPosition);
			while (leftPosition != 0) {
				char nextChar = text.at(leftPosition - 1);

				if (std::find(stopChars.begin(), stopChars.end(), nextChar) == stopChars.end()){
					break;
				}

				leftPosition--;
			}

			std::cout << leftPosition << " -pos" << std::endl;
		}
	} else {
		clickCount = 0;
	}
		
	highlightChar = cursorPosition;

	// TextInputHandler::LinkForInput(&text);
}

bool StopChar(char c){
	return c == ' ' || c == '\n' || c == '.' || c == ',';
}

void Elements::TextField::Update(){
	int prevCursorPosition = cursorPosition;

	if (selectedElement == this){

		if (!hasBeenReleased){
			if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)){
				hasBeenReleased = true;
			} else {
				Vector2 tMPos = GetMousePosition();
				Vector2 mPos = {tMPos.x - savedOffset.x, tMPos.y - savedOffset.y};
				int nCursorPos = GetCursorIndex(mPos);

				if (mPos.x < 0 || mPos.y < 0)
					nCursorPos = 0;
				
				if (highlightChar != cursorPosition || nCursorPos != cursorPosition)
					cursorPosition = nCursorPos;
			}
		}

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
		
		if (GetKeyPressed() > 0)
			highlightChar = -1;

		if (KeyInput::KeyActive(KEY_BACKSPACE))
		{
			highlightChar = -1;
			if (cursorPosition != 0){
				if (IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)){
					char curChar = text.at(cursorPosition - 1);
					if (curChar == '\n'){
						text.erase(text.begin() + cursorPosition-1);
						cursorPosition--;
					}else{
						while (cursorPosition != 0){
							char nextChar = text.at(cursorPosition - 1);

							if (!StopChar(curChar) && StopChar(nextChar)){
								break;
							}

							text.erase(text.begin() + cursorPosition-1);
							cursorPosition--;

							curChar = nextChar;
						}
					}
				}else{
					text.erase(text.begin() + cursorPosition-1);
					cursorPosition--;
				}
			}
		}

		if (KeyInput::KeyActive(KEY_DELETE))
		{
			if (cursorPosition != text.size()){
				if (IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)){
					char curChar = text.at(cursorPosition);
					if (curChar == '\n'){
						text.erase(text.begin() + cursorPosition);
					}else{
						while (cursorPosition != text.size()){
							char nextChar = text.at(cursorPosition);

							if ((StopChar(curChar) && !StopChar(nextChar)) || nextChar == '\n'){
								break;
							}

							text.erase(text.begin() + cursorPosition);

							curChar = nextChar;
						}
					}
				}else{
					text.erase(text.begin() + cursorPosition);
				}
			}
		}

		if (KeyInput::KeyActive(KEY_ENTER))
		{
			float nHeight = MeasureTextHeightWithChar('\n');
			if (nHeight <= size.y) {
				text.insert(text.begin()+cursorPosition, '\n');
				cursorPosition++;
			}
		}

		if (KeyInput::KeyActive(KEY_LEFT) && cursorPosition != 0){
			if (IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)){
				char startChar = text.at(cursorPosition - 1);
				if (startChar == '\n'){
					cursorPosition--;
				} else {
					char curChar = text.at(cursorPosition-1);

					while (cursorPosition != 0){
						char nextChar = text.at(cursorPosition-1);

						if (!StopChar(curChar) && StopChar(nextChar)){
							break;
						}

						cursorPosition--;
						curChar = nextChar;
					}
				}
			} else {
				cursorPosition--;
			}
		}

		if (KeyInput::KeyActive(KEY_RIGHT) && cursorPosition != text.size()){
			if (IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)){
				char startChar = text.at(cursorPosition);
				if (startChar == '\n'){
					cursorPosition++;
				} else {
					char curChar = text.at(cursorPosition);

					while (cursorPosition != text.size()){
						char nextChar = text.at(cursorPosition);

						if ((StopChar(curChar) && !StopChar(nextChar)) || nextChar == '\n'){
							break;
						}

						cursorPosition++;
						curChar = nextChar;
					}
				}
			} else {
				cursorPosition++;
			}
		}

		if (prevCursorPosition != cursorPosition){
			savedX = -1;
		}

		if (KeyInput::KeyActive(KEY_UP)){
			if (savedX == -1){
				savedX = curX;
				savedY = curY;
			}

			float futureSavedY = savedY - textLineSpacing;
			
			if (futureSavedY > 0)
				savedY = futureSavedY;

			if (futureSavedY < 0){
				cursorPosition = 0;
				savedX = 0;
			}
			else
				cursorPosition = GetCursorIndex({savedX, savedY});
		}

		if (KeyInput::KeyActive(KEY_DOWN)){
			if (savedX == -1){
				savedX = curX;
				savedY = curY;
			}

			int prevSavedCursor = cursorPosition;
			savedY += textLineSpacing;
			
			cursorPosition = GetCursorIndex({savedX, savedY});
			
			if (savedX > curX){
				if (cursorPosition == prevSavedCursor){
					savedY -= textLineSpacing;
					savedX = curX;
				}
			} else {
				if (cursorPosition == text.size()){
					savedY -= textLineSpacing;
					savedX = curX;
				}
			}
		}
	}
}


bool InBetween(int val, int v1, int v2){
	if (v1 > v2)
		return val < v1 && val > v2-1;
	else
		return val < v2 && val > v1-1;
}

// need to make it so that only one element can be selected at a time and then make it work that way around instead of making TextInputHandler edit a pointer...

void Elements::TextField::DrawCodepointAt(Vector2 origin, std::vector<int> codepointBuffer, int& curIndex, float &textOffsetX, float textOffsetY, bool &didDrawCursor){
	Font font = FontManager::GetFont(fontType, fontSize);

	if (codepointBuffer.size() > 0){
		for (int cp : codepointBuffer){
			int index = GetGlyphIndex(font, cp);
			float cWidth = (font.glyphs[index].advanceX == 0 ? (float)font.recs[index].width : (float)font.glyphs[index].advanceX) + spacing;

			if (highlightChar != cursorPosition && highlightChar != -1 && InBetween(curIndex, highlightChar, cursorPosition)){
				DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, cWidth, fontSize, GRAY);
			}

			DrawTextCodepoint(font, cp, { origin.x + textOffsetX, origin.y + textOffsetY}, fontSize, BLACK);
			
			// std::cout << curIndex << " is " << (curIndex == cursorPosition) << std::endl;

			if (curIndex == cursorPosition && !didDrawCursor){
				didDrawCursor = true;
				int effectiveX = origin.x + (textOffsetX > this->size.x-1 ? this->size.x-1 : textOffsetX);
				DrawRectangle(effectiveX, origin.y + textOffsetY, 1, fontSize, BLACK);
				curX = effectiveX;
				curY = textOffsetY + fontSize / 2;
			}

			curIndex++; // idk if this should be here or before, but whatever...
			textOffsetX += cWidth;
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
			int nextCodepoint = GetCodepointNext(&text[i+1], &codepointByteCount);
			int nextCodepointIndex = GetGlyphIndex(font, nextCodepoint);
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
				int effectiveX = origin.x + (textOffsetX > this->size.x-1 ? this->size.x-1 : textOffsetX);
				DrawRectangle(effectiveX, origin.y + textOffsetY, 1, fontSize, BLACK);
				curX = effectiveX;
				curY = textOffsetY + fontSize / 2;
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
			// std::cout << curLineWidth + nextCharWidth + codepointBufferWidth << " > " << (this->size.x) << std::endl;
				if (curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x && wrapping != 2 && !(codepoint == ' ' || codepoint == '\t')){
					// std::cout << "fg" << std::endl;
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
								int effectiveX = origin.x + (textOffsetX > this->size.x-1 ? this->size.x-1 : textOffsetX);
								DrawRectangle(effectiveX, origin.y + textOffsetY, 1, fontSize, BLACK);
								curX = effectiveX;
								curY = textOffsetY + fontSize / 2;
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

	if (!didDrawCursor){
		int effectiveX = origin.x + (textOffsetX > this->size.x-1 ? this->size.x-1 : textOffsetX);
		DrawRectangle(effectiveX, origin.y + textOffsetY, 1, fontSize, BLACK);
		curX = effectiveX;
		curY = textOffsetY + fontSize / 2;
	}

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
			int nextCodepoint = GetCodepointNext(&text[i+1], &codepointByteCount);
			int nextCodepointIndex = GetGlyphIndex(font, nextCodepoint);
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
				if (curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x && wrapping != 2 && !(codepoint == ' ' || codepoint == '\t')){
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

			if (textOffsetX + codepointWidth/2 >= localMousePosition.x && textOffsetY <= localMousePosition.y && textOffsetY + fontSize >= localMousePosition.y){
				curX = textOffsetX;
				curY = textOffsetY + fontSize / 2;
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
			int nextCodepoint = GetCodepointNext(&text[i+1], &codepointByteCount);
			int nextCodepointIndex = GetGlyphIndex(font, nextCodepoint);
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
				// std::cout << textOffsetY << " | " << localMousePosition.y << " - " << fontSize << std::endl;
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
				if (curLineWidth + nextCharWidth + codepointBufferWidth > this->size.x && wrapping != 2 && !(codepoint == ' ' || codepoint == '\t')){
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

	curX = textOffsetX;
	curY = textOffsetY + fontSize / 2;
	return text.size(); // or -1 idk yet
}
