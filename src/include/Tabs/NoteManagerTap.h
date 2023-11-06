#pragma once

#include "Tab.h"

namespace Tabs { // managing note types
	class NoteManagerTap : public Tab {
	public:
		NoteManagerTap();
		void Draw(int x, int y, int w, int h) override;
		void Update() override;
	};
}