#include "ElementToolboxTile.h"

#include "NoteManagerTap.h"

#include "FontManager.h"

#include <raylib.h>

namespace Tabs::Tiles{
	void ElementToolboxTile::Draw(int x, int y, int w, int h){
		DrawRectangle(x, y, w, h, MAGENTA);
		DrawRectangleLines(x, y, w, h, BLACK);

		int fontSize = 24; // get default ui font ig
		std::string fontType = "Vera"; // get default ui font ig
		Font mainFont = FontManager::GetFont(fontType, fontSize, 0);
        DrawTextEx(mainFont, "Element Toolbox", {(float)x+5, (float)y+5}, fontSize, 0, BLACK);
		DrawRectangleLines(x, y, w, 34, BLACK);
	}

	void ElementToolboxTile::MousePressed(int x, int y){
		 std::cout << Tabs::NoteManagerTap::editingElement << std::endl;
	}
}
