#pragma once

#include <raylib.h>
#include <string>

class Element {
public:
	std::string Name;
	Vector2 position;
	Vector2 size;

	virtual void Draw() = 0;
	virtual void Update() {};
	virtual void MousePressed(Vector2 position) {};
};