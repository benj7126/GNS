#include "Elements/SimpleTextField.h"

#include "FontManager.h"

#include <iostream>

void Elements::SimpleTextField::Draw(Vector2 offset) {
	// std::cout << offset.x + position.x << offset.y + position.y << size.x << size.y << std::endl;
	int fontSize = 12;
	Font font = FontManager::GetFont("", fontSize);
	
	DrawRectangle(offset.x + position.x, offset.y + position.y, size.x, size.y, ORANGE);

	DrawTextEx(font, text.c_str(), {offset.x + position.x, offset.y + position.y}, fontSize, 0, BLACK);
}

void Elements::SimpleTextField::Update(){
	if (selectedElement == this){
		int key = GetCharPressed();

		while (key > 0)
		{
			if ((key >= 32) && (key <= 125))
			{
				text += (char)key;
			}

			key = GetCharPressed();
		}

		if (IsKeyPressed(KEY_BACKSPACE))
		{
			if (!text.empty())
				text.pop_back();
		}
	}
}

void Elements::SimpleTextField::MousePressed(Vector2 pos) {
	std::cout << "test" << std::endl;
	selectedElement = this;
	// TextInputHandler::LinkForInput(&text);
}


// need to make it so that only one element can be selected at a time and then make it work that way around instead of making TextInputHandler edit a pointer...