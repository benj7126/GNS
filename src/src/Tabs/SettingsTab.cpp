#include "Tabs/SettingsTab.h"

#include <memory>

Settings Tabs::SettingsTab::settings{};

Settings::Settings() {
	// settings.insert({ "EditMode", std::make_shared<settingValue<bool>>(editMode) }); --- this is will be toggelable with keybind
	settingHolder.insert("DefaultFontType", std::make_shared<DataStruct<std::string>>(&defaultFontType));

	std::shared_ptr<DataStructHolder> _tabSettings = std::make_shared<DataStructHolder>();
	_tabSettings->insert("fontSize", std::make_shared<DataStruct<int>>(&tabSettings.fontSize));
	_tabSettings->insert("paddingUp", std::make_shared<DataStruct<float>>(&tabSettings.paddingUp));
	_tabSettings->insert("paddingDown", std::make_shared<DataStruct<float>>(&tabSettings.paddingDown));
	_tabSettings->insert("paddingLeft", std::make_shared<DataStruct<float>>(&tabSettings.paddingLeft));
	_tabSettings->insert("paddingRight", std::make_shared<DataStruct<float>>(&tabSettings.paddingRight));
	_tabSettings->insert("paddingRight", std::make_shared<DataStruct<std::string>>(&tabSettings.fontType));
	settingHolder.insert("TabSettings", _tabSettings);
}

Tabs::SettingsTab::SettingsTab() {
	Name = "Settings";

	//*((SettingValue<int>*)((SettingStructValue*)settings.mappedSettings["TabSettings"].get())->subSettings["fontSize"].get())->object = 100;
}

void Tabs::SettingsTab::Update(int x, int y) {}
