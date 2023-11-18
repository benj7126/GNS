#include "Elements/TextField.h"

#include "FontManager.h"

#include <iostream>

void Elements::TextField::Draw(Vector2 offset) {
	// std::cout << offset.x + position.x << offset.y + position.y << size.x << size.y << std::endl;
	// std::cout << textFont << std::endl;
	
	Font font = FontManager::GetFont(fontType, fontSize);
	
	size.y = fontSize; // might want to set this when you change size instead, as to make it constant somehow...
	DrawRectangle(offset.x + position.x, offset.y + position.y, size.x, size.y, ORANGE); // size is {0, 0}...
	
	CustomTextDraw({offset.x + position.x, offset.y + position.y});
}

void Elements::TextField::CustomTextDraw(Vector2 origin){
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
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        if (codepoint == '\n')
        {
            // NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup
            textOffsetY += textLineSpacing;
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint(font, codepoint, { origin.x + textOffsetX, origin.y + textOffsetY }, fontSize, BLACK);
            }

			if (curIndex == cursorPosition)
				DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);

			curIndex++; // idk if this should be here or before, but whatever...

            if (font.glyphs[index].advanceX == 0) textOffsetX += ((float)font.recs[index].width + spacing);
            else textOffsetX += ((float)font.glyphs[index].advanceX + spacing);
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }

	if (curIndex == cursorPosition)
		DrawRectangle(origin.x + textOffsetX, origin.y + textOffsetY, 1, fontSize, BLACK);
}

int Elements::TextField::GetCursorIndex(Vector2 localMousePosition){
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

void Elements::TextField::Update(){
	if (selectedElement == this){
		int key = GetCharPressed();

		while (key > 0)
		{
			if ((key >= 32) && (key <= 125))
			{
				Font font = FontManager::GetFont(fontType, fontSize);
    			char *charPointer = (char *)malloc(2 * sizeof(char));
    			charPointer[0] = (char)key;
				charPointer[1] = '\0';

				std::cout << MeasureTextEx(font, text.c_str(), fontSize, spacing).x << " - " << MeasureTextEx(font, charPointer, fontSize, spacing).x << std::endl;

				if (MeasureTextEx(font, text.c_str(), fontSize, spacing).x + MeasureTextEx(font, charPointer, fontSize, spacing).x <= size.x){
					// only insert if it dosent go out of bounds...
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

		if (IsKeyPressed(KEY_LEFT))
			cursorPosition = cursorPosition == 0 ? 0 : cursorPosition-1;

		if (IsKeyPressed(KEY_RIGHT))
			cursorPosition = cursorPosition == text.size() ? text.size() : cursorPosition+1;
	}
}

void Elements::TextField::MousePressed(Vector2 pos) {
	std::cout << "test char pos thingy: " << GetCursorIndex(pos) << std::endl;
	cursorPosition = GetCursorIndex(pos);
	selectedElement = this;
	// TextInputHandler::LinkForInput(&text);
}


// need to make it so that only one element can be selected at a time and then make it work that way around instead of making TextInputHandler edit a pointer...