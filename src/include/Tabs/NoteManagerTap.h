#pragma once

#include "Tabs/TiledTab.h"
#include "Note.h"

#include <string>
#include <unordered_map>

struct NoteData{
	std::string name;
	Vector2 size;
	std::vector<std::string> elements; // but their string forms ofc...
};

namespace Tabs { // managing note types - theres only one of these...
	class NoteManagerTap : public TiledTab {
	public:
		static std::unordered_map<std::string, NoteData> allNotes;
		static std::string editingElement;
		NoteManagerTap();
	};
}
