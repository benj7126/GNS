#include "../include/Tab.h"
#include "../include/FontManager.h"
#include "../include/Tabs/SettingsTab.h"

#include <raylib.h>

#include <iostream>

Vector2 Tab::CalculateTab(Vector2 xAndHeight) {
	std::string fontType = Tabs::SettingsTab::settings.tabSettings.fontType;
	int fontSize = Tabs::SettingsTab::settings.tabSettings.fontSize;
	Font font = FontManager::GetFont(fontType, fontSize);
	Vector2 TextSize = MeasureTextEx(font, Name.c_str(), fontSize, 0);

	xAndHeight.x += Tabs::SettingsTab::settings.tabSettings.paddingLeft;
	xAndHeight.x += Tabs::SettingsTab::settings.tabSettings.paddingRight + TextSize.x;
	xAndHeight.y = xAndHeight.y < TextSize.y + Tabs::SettingsTab::settings.tabSettings.paddingUp + Tabs::SettingsTab::settings.tabSettings.paddingDown ?
		TextSize.y + Tabs::SettingsTab::settings.tabSettings.paddingUp + Tabs::SettingsTab::settings.tabSettings.paddingDown : xAndHeight.y;

	return xAndHeight;
}

Vector2 Tab::DrawTab(Vector2 xAndHeight, bool selected) {
	std::string fontType = Tabs::SettingsTab::settings.tabSettings.fontType;
	int fontSize = Tabs::SettingsTab::settings.tabSettings.fontSize;
	Font font = FontManager::GetFont(fontType, fontSize);
	Vector2 TextSize = MeasureTextEx(font, Name.c_str(), fontSize, 0);

	DrawRectangle((int)xAndHeight.x, 0,
		TextSize.x + Tabs::SettingsTab::settings.tabSettings.paddingRight + Tabs::SettingsTab::settings.tabSettings.paddingLeft,
		TextSize.y + Tabs::SettingsTab::settings.tabSettings.paddingUp + Tabs::SettingsTab::settings.tabSettings.paddingDown, selected ? GRAY : WHITE);

	DrawRectangleLines((int)xAndHeight.x, 0,
		TextSize.x + Tabs::SettingsTab::settings.tabSettings.paddingRight + Tabs::SettingsTab::settings.tabSettings.paddingLeft,
		TextSize.y + Tabs::SettingsTab::settings.tabSettings.paddingUp + Tabs::SettingsTab::settings.tabSettings.paddingDown, BLACK);

	xAndHeight.x += Tabs::SettingsTab::settings.tabSettings.paddingLeft;

	DrawTextEx(font, Name.c_str(), { xAndHeight.x, Tabs::SettingsTab::settings.tabSettings.paddingUp }, fontSize, 0, BLACK);

	xAndHeight.x += Tabs::SettingsTab::settings.tabSettings.paddingRight + TextSize.x;
	xAndHeight.y = TextSize.y + Tabs::SettingsTab::settings.tabSettings.paddingUp + Tabs::SettingsTab::settings.tabSettings.paddingDown;

	return xAndHeight;
}