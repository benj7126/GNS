#pragma once

#include <string>

#include <raylib.h>

class Tab {
protected:
	std::string Name;

public:
	Vector2 CalculateTab(Vector2 xAndHeight);
	Vector2 DrawTab(Vector2 xAndHeight, bool selected);
	virtual void Draw(int x, int y, int w, int h) = 0;
	virtual void MousePressed(int x, int y) {};
};