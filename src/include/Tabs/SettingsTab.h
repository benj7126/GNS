#pragma once

#include "Tab.h"

#include <optional>
#include <string>
#include <memory>
#include <map>

struct SettingValueBase {};

template<typename T>
struct SettingValue : public SettingValueBase {
	std::string type;
    T* object;

    SettingValue(T* objectIn) {
		object = objectIn;
		type = typeid(T).name();
    }
};

struct SettingStructValue : public SettingValueBase {
	std::map<std::string, std::shared_ptr<SettingValueBase>> subSettings{};
};

struct TabSettings {
	int fontSize = 24;
	float paddingUp = 3;
	float paddingDown = 3;
	float paddingLeft = 6;
	float paddingRight = 6;
	std::string fontType = ""; // "" = default

	// something about spacing?
};

struct Settings {
	bool editMode = false;
	std::string defaultFontType = "Vera";

	TabSettings tabSettings{};

	std::map<std::string, std::shared_ptr<SettingValueBase>> mappedSettings{};

	Settings();
};

namespace Tabs {
	class SettingsTab : public Tab {
	public:
		SettingsTab();

		static Settings settings;

		void Draw(int x, int y, int w, int h) override {}
		void Update() override;
	};
}