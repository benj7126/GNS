#include "../../include/Tabs/WorkspaceTab.h"

Tabs::WorkspaceTab::WorkspaceTab() {
	Name = "New Tab"; // for now, need to be the name of the current workspace
}

void Tabs::WorkspaceTab::Draw(int x, int y, int w, int h) {
	if (workspace == nullptr) {
		// let user pick workspace type
		workspace = std::make_shared<Workspaces::BlankWorkspace>();
		((Workspaces::BlankWorkspace*)workspace.get())->TestAddNote();
	}
	else {
		workspace.get()->Draw(x, y, w, h);
	}
}

void Tabs::WorkspaceTab::MousePressed(int x, int y) {
	if (workspace != nullptr) {
		workspace.get()->MousePressed(x, y);
	}
}