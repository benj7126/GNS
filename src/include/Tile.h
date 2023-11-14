#pragma once

#include <iostream>

#include <raylib.h>

class Tile {
public:
	virtual void Draw(int x, int y, int w, int h) { DrawRectangle(x, y, w, h, WHITE); DrawRectangleLines(x, y, w, h, BLACK); };
	virtual void Update() {};
	virtual void MousePressed(int x, int y) {};
};