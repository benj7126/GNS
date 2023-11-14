#include "EditWindowTile.h"

#include <raylib.h>

void Tabs::Tiles::EditWindowTile::Draw(int x, int y, int w, int h){
	DrawRectangle(x, y, w, h, ORANGE);
	DrawRectangleLines(x, y, w, h, BLACK);

	DrawRectangleLines(x + sx-5, y + sy-5, 10, 10, WHITE);
}

void Tabs::Tiles::EditWindowTile::MousePressed(int x, int y){
	std::cout << x << " | " << y << std::endl;
	sx = x;
	sy = y;
}