#pragma once

#include "Tab.h"
#include "Workspace.h"

#include "Workspaces/BlankWorkspace.h"

#include <memory>

namespace Tabs {
	class WorkspaceTab : public Tab {
		std::shared_ptr<Workspace> workspace;

	public:
		WorkspaceTab();

		void Draw(int x, int y, int w, int h) override;
		void MousePressed(int x, int y) override;
	};
}