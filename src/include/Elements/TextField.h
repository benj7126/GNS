#pragma once

#include "Element.h"

#include <string>
#include <vector>

namespace Elements { // managing note types
	class TextField : public Element {
	protected:
		std::string text = "";
		int cursorPosition = 0;

		int fontSize = 16;
		std::string fontType = "";

		int textLineSpacing = 15;
		int spacing = 0;

		Color textColor = BLACK;
		Color backgroundColor = WHITE;

		bool charWrapNextLine = false;

		float curX = 0;
		float curY = 0;
		float savedX = -1;
		float savedY = -1;

		/* dont know if its possible
		int paddingUp = 5;
		int paddingDown = 5;
		int paddingLeft = 5;
		int paddingRight = 5;
		*/

		void InternalDraw(Vector2 offset);

	public:
		void Draw(Vector2 offset) override{
			BeginScissorMode(offset.x + position.x, offset.y + position.y, size.x, size.y);
			InternalDraw(offset);
			EndScissorMode();
		}
		void Update() override;
		void MousePressed(Vector2 position) override;

		int GetCursorIndex(Vector2 localMousePosition);
		void DrawCodepointAt(Vector2 position, std::vector<int> codepointBuffer, int& curIndex, float &textOffsetX, float textOffsetY, bool &didDrawCursor);
		int ScanCodepointsAt(Vector2 localMousePosition, std::vector<int> codepointBuffer, int& curIndex, float &textOffsetX, float textOffsetY);
		void CustomTextDraw(Vector2 origin);
		float MeasureTextHeightWithChar(char extraChar);

		int wrapping = 0; // 0: char 1: word | 2: none - needs to be private; public for testing
	};
}