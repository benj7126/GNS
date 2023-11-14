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

namespace Tabs { // managing note types
	class NoteManagerTap : public TiledTab {
	private:
		static std::unordered_map<std::string, NoteData> allNotes;

	public:
		NoteManagerTap();
	};
}