#include "EditWindowTile.h"

#include <raylib.h>

void Tabs::Tiles::EditWindowTile::Draw(int x, int y, int w, int h){
	DrawRectangle(x, y, w, h, ORANGE);
	DrawRectangleLines(x, y, w, h, BLACK);
}

void Tabs::Tiles::EditWindowTile::MousePressed(int x, int y){
	
}