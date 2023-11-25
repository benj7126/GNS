#include "ApplicationManager.h"

#include "Tabs/WorkspaceTab.h"
#include "Tabs/NoteManagerTap.h"
#include "Tabs/SettingsTab.h"

#include "KeyInput.h"
int KeyInput::curKey = -1;
float KeyInput::updateCooldown = 0;

#include <memory>
#include <raylib.h>

#include <iostream>

ApplicationManager::ApplicationManager() {
	tabs.push_back(std::make_shared<Tabs::WorkspaceTab>());
	tabs.push_back(std::make_shared<Tabs::NoteManagerTap>());
	tabs.push_back(std::make_shared<Tabs::SettingsTab>());

	lockedTapCount = 2; // one for the settingsTab
}

void ApplicationManager::AddWorkspace() {
	tabs.insert(tabs.end() - lockedTapCount, std::make_shared<Tabs::WorkspaceTab>());
}

void ApplicationManager::Draw() {
	// DrawRectangle(0, 0, W, H, BLUE);
	
	Vector2 xAndHeight{0, 0};
	int tapIDX = 0;
	for (std::shared_ptr<Tab> tab : tabs) {
		xAndHeight = tab->DrawTab(xAndHeight, tapIDX == curTap);
		tapIDX++;
	}

	tabs[curTap].get()->Draw(0, xAndHeight.y, W, H - xAndHeight.y);
}

void ApplicationManager::Update() {
	KeyInput::UpdateKeyInput();

	Vector2 mouse = GetMousePosition();
	Vector2 xAndHeight{ 0, 0 };

	for (std::shared_ptr<Tab> tab : tabs) {
		xAndHeight = tab->CalculateTab(xAndHeight);
	}

	tabs[curTap].get()->Update(mouse.x, mouse.y - xAndHeight.y);

	if (IsMouseButtonPressed(0)) {
		// reset ~input~ well, selected now
		//TextInputHandler::LinkForInput(nullptr);
		Element::ClearSelected();

		bool allowWokspacePress = true;

		// update tabs
		xAndHeight = { 0, 0 };
		int tapIDX = 0;

		for (std::shared_ptr<Tab> tab : tabs) {
			int lastX = xAndHeight.x;
			xAndHeight = tab->CalculateTab(xAndHeight);

			if (lastX <= mouse.x && xAndHeight.x > mouse.x && 0 <= mouse.y && xAndHeight.y > mouse.y) {
				curTap = tapIDX;
			}

			if (xAndHeight.y > mouse.y)
				allowWokspacePress = false;

			tapIDX++;
		}

		// pass to workspace
		if (allowWokspacePress) {
			tabs[curTap].get()->MousePressed(mouse.x, mouse.y - xAndHeight.y);
		}
	}
	// TextInputHandler::Update();
}