#include "Tabs/NoteManagerTap.h"
std::string Tabs::NoteManagerTap::editingElement = "TestElement";
std::unordered_map<std::string, NoteData> Tabs::NoteManagerTap::allNotes = {};
#include "ElementInspectorTile.h"
#include "ElementToolboxTile.h"
#include "ShowWindowTile.h"


Tabs::NoteManagerTap::NoteManagerTap() {
	Name = "Note Manager";

	tile.split(true, {0.2, 0.8}, {
		std::make_shared<Tabs::Tiles::ElementToolboxTile>(),
		std::make_shared<Tabs::Tiles::ShowWindowTile>(),
		std::make_shared<Tabs::Tiles::ElementInspectorTile>(),
	});
	// tile.tp1->split(false, 0.2, std::make_shared<Tabs::Tiles::EditWindowTile>());
	// tile.tp1->tp2->split(true, 0.6, std::make_shared<Tabs::Tiles::EditWindowTile>());
}
