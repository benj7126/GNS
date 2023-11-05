#include "../../include/Elements/SimpleTextField.h"

#include "../../include/TextInputHandler.h"
#include "../../include/FontManager.h"

void Elements::SimpleTextField::Draw() {
	int fontSize = 12;
	Font font = FontManager::GetFont("", fontSize);
	
	DrawRectangle(0, 0, size.x, size.y, ORANGE);

	DrawTextEx(GetFontDefault(), text.c_str(), {0, 0}, 0, 0, BLACK);
}

void Elements::SimpleTextField::MousePressed(Vector2 pos) {
	TextInputHandler::LinkForInput(&text);
}