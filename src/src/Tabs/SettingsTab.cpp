#include "Tabs/SettingsTab.h"

#include <memory>

Settings Tabs::SettingsTab::settings{};

Settings::Settings() {
	// settings.insert({ "EditMode", std::make_shared<settingValue<bool>>(editMode) }); --- this is will be toggelable with keybind
	mappedSettings.insert({ "DefaultFontType", std::make_shared<SettingValue<std::string>>(&defaultFontType) });

	std::shared_ptr<SettingStructValue> _tabSettings = std::make_shared<SettingStructValue>();
	_tabSettings->subSettings.insert({ "fontSize", std::make_shared<SettingValue<int>>(&tabSettings.fontSize) });
	_tabSettings->subSettings.insert({ "paddingUp", std::make_shared<SettingValue<float>>(&tabSettings.paddingUp) });
	_tabSettings->subSettings.insert({ "paddingDown", std::make_shared<SettingValue<float>>(&tabSettings.paddingDown) });
	_tabSettings->subSettings.insert({ "paddingLeft", std::make_shared<SettingValue<float>>(&tabSettings.paddingLeft) });
	_tabSettings->subSettings.insert({ "paddingRight", std::make_shared<SettingValue<float>>(&tabSettings.paddingRight) });
	_tabSettings->subSettings.insert({ "paddingRight", std::make_shared<SettingValue<std::string>>(&tabSettings.fontType) });
	mappedSettings.insert({ "TabSettings", _tabSettings });
}

Tabs::SettingsTab::SettingsTab() {
	Name = "Settings";

	//*((SettingValue<int>*)((SettingStructValue*)settings.mappedSettings["TabSettings"].get())->subSettings["fontSize"].get())->object = 100;
}

void Tabs::SettingsTab::Update() {}