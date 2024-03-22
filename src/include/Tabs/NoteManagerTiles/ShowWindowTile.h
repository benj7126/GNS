#pragma once

#include "Tile.h"
#include "TextField.h"

namespace Tabs::Tiles { // managing note types
	class ShowWindowTile : public Tile {
	public:
		Elements::TextField noteName = {};

		void Update() override;
		void Draw(int x, int y, int w, int h) override;
		void MousePressed(int x, int y) override;
	};
}
