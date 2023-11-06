#include "Elements/SimpleTextField.h"

#include "TextInputHandler.h"
#include "FontManager.h"

void Elements::SimpleTextField::Draw() {
	int fontSize = 12;
	Font font = FontManager::GetFont("", fontSize);
	
	DrawRectangle(0, 0, size.x, size.y, ORANGE);

	DrawTextEx(GetFontDefault(), text.c_str(), {0, 0}, 0, 0, BLACK);
}

void Elements::SimpleTextField::MousePressed(Vector2 pos) {
	TextInputHandler::LinkForInput(&text);
}

// need to make it so that only one element can be selected at a time and then make it work that way around instead of making TextInputHandler edit a pointer...