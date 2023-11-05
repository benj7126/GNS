#pragma once

#include "../Element.h"

#include <string>

namespace Elements { // managing note types
	class SimpleTextField : public Element {
	protected:
		std::string text;

	public:
		void Draw() override;
		void MousePressed(Vector2 position) override;
	};
}