#include "ShowWindowTile.h"

#include "NoteManagerTap.h"

#include <raylib.h>

namespace Tabs::Tiles{
	void ShowWindowTile::Draw(int x, int y, int w, int h){
		DrawRectangle(x, y, w, h, WHITE);
		DrawRectangleLines(x, y, w, h, BLACK);
	}

	void ShowWindowTile::MousePressed(int x, int y){
		 std::cout << Tabs::NoteManagerTap::editingElement << std::endl;
	}
}
