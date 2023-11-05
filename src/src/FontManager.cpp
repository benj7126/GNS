#include "../include/FontManager.h"

#include "../include/Tabs/SettingsTab.h"

std::unordered_map<std::string, Font> FontManager::fontMap{};

Font FontManager::GetFont(std::string fontName, int size, int filter) {
	if (fontName == "") {
		fontName = Tabs::SettingsTab::settings.defaultFontType;
	}

	std::string filePath = "assets/Fonts/" + fontName + ".ttf";
	fontName = fontName + " | " + std::to_string(filter) + " | " + std::to_string(size);

	if (fontMap.count(fontName) == 0) {
		Font font = LoadFontEx(filePath.c_str(), size, 0, 0);
		SetTextureFilter(font.texture, filter);

		fontMap.insert({ fontName, font});
	}

	return fontMap.at(fontName);
}