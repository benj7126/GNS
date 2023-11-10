#pragma once

#include "Element.h"

#include <string>

namespace Elements { // managing note types
	class SimpleTextField : public Element {
	protected:
		std::string text = "";
		int cursorPosition = 0;

		int fontSize = 16;
		std::string fontType = "";

		int textLineSpacing = 15;
		int spacing = 0;

		Color textColor = BLACK;
		Color backgroundColor = WHITE;

	public:
		void Draw(Vector2 offset) override;
		void Update() override;
		void MousePressed(Vector2 position) override;

		int GetCursorIndex(Vector2 localMousePosition);
		void CustomTextDraw(Vector2 origin);
	};
}