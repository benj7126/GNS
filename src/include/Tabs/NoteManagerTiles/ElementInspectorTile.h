#pragma once

#include "Tile.h"

namespace Tabs::Tiles { // managing note types
	class ElementInspectorTile : public Tile {
		void Draw(int x, int y, int w, int h) override;
		void MousePressed(int x, int y) override;
	};
}
