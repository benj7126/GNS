#pragma once

#include "IEditable.h"

#include <raylib.h>
#include <string>
#include <vector>

namespace Elements::Interfaces { // managing note types
	class ILabel : public IEditable {
	protected:
		std::string text = "Label";

		int fontSize = 16;
		std::string fontType = "";

		int textLineSpacing = 15;
		int spacing = 0;

		Color textColor = BLACK;
		Color backgroundColor = WHITE;

		bool charWrapNextLine = false;

	public:
		int wrapping = 0; // 0: char 1: word | 2: none - needs to be private; public for testing
	};
}
