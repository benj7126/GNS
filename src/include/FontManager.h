#pragma once

#include <unordered_map>
#include <raylib.h>
#include <string>


class FontManager {
private:
	static std::unordered_map<std::string, Font> fontMap;

public:
	static Font GetFont(std::string fontName, int size, int filter = TEXTURE_FILTER_POINT);
	// static Font GetDefaultFont(int size, int filter = TEXTURE_FILTER_POINT); - just don GetFont("", ...
};