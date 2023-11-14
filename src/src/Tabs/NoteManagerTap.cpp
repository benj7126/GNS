#include "Tabs/NoteManagerTap.h"

#include "EditWindowTile.h"

Tabs::NoteManagerTap::NoteManagerTap() {
	Name = "Note Manager";

	tile.split(true, 0.5, std::make_shared<Tabs::Tiles::EditWindowTile>());
	tile.tp1 = std::make_shared<TilePair>(std::make_shared<Tabs::Tiles::EditWindowTile>());
	tile.tp1->split(false, 0.2, std::make_shared<Tabs::Tiles::EditWindowTile>());
	tile.tp1->tp2->split(true, 0.6, std::make_shared<Tabs::Tiles::EditWindowTile>());
}