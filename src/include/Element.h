#pragma once

#include <raylib.h>
#include <string>


class Element {
protected:
	static Element* selectedElement;

public:
	// static std::unordered_map<std::string, Type> elementTypes;
	static void ClearSelected() { selectedElement = nullptr; };


	std::string Name;
	Vector2 position;
	Vector2 size;

	virtual void Draw(Vector2 offset) = 0;
	virtual void Update() {};
	virtual void MousePressed(Vector2 position) {};
};
