/*
#pragma once

#include "Element.h"

#include <string>
#include <vector>

namespace Elements { // managing note types
	class Button : public Element {
	protected:
		std::string text = "";

		int fontSize = 16;
		std::string fontType = "";

		int textLineSpacing = 15;
		int spacing = 0;

		Color textColor = BLACK;
		Color backgroundColor = WHITE;

		bool charWrapNextLine = false;

	public:
		void Draw(Vector2 offset) override;
		void Update() override;
		void MousePressed(Vector2 position) override;
	};
}
*/
