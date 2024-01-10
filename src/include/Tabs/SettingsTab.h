#pragma once

#include "Tab.h"
#include "DataStruct.h"

#include <optional>
#include <string>

struct TabSettings {
	int fontSize = 24;
	float paddingUp = 3;
	float paddingDown = 3;
	float paddingLeft = 6;
	float paddingRight = 6;
	std::string fontType = ""; // "" = default

	// something about spacing?
};

struct NoteManagerSettings {
};

struct Settings {
	bool editMode = false;
	std::string defaultFontType = "Vera";

	TabSettings tabSettings{};
	//NoteManagerSettings noteManagerSettings{};

	DataStructHolder settingHolder{};

	Settings();
};

namespace Tabs {
	class SettingsTab : public Tab {
	public:
		SettingsTab();

		static Settings settings;

		void Draw(int x, int y, int w, int h) override {}
		void Update(int x, int y) override;
	};
}
