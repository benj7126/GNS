#pragma once

#include "Element.h"

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>

#include <iostream>

class Note {
public:
	std::string name; // more like a yupe
	Vector2 size;
	std::vector<std::shared_ptr<Element>> elements{};

	Note(std::string name, Vector2 size) : name{name}, size{size} {

	}

	virtual void Draw(Vector2 position) {
		// DrawRectangle(position.x, position.y, size.x, size.y, WHITE); // might want this to be an element idk yet...
		DrawRectangleLines(position.x, position.y, size.x, size.y, BLACK);

		for (std::shared_ptr<Element> e : elements) {
			e.get()->Draw(position);
		}
	};

	virtual void Update() {
		for (std::shared_ptr<Element> e : elements) {
			e.get()->Update();
		}
	};

	virtual void MousePressed(Vector2 position) {
		for (std::shared_ptr<Element> e : elements) {
			if (e->position.x < position.x && e->position.x + e->size.x > position.x && e->position.y < position.y && e->position.y + e->size.y > position.y){
				e.get()->MousePressed({position.x - e->position.x, position.y - e->position.y});
			}
		}
	};
};