#pragma once

#include "Tabs/TiledTab.h"
#include "Note.h"

#include "ElementType.h"

#include <string>
#include <unordered_map>

struct NoteData{
	std::string name{};
	Vector2 size{};
	std::vector<int> elements{}; // but their string forms ofc...

	Note genNote(){
		Note n{name, size};

		for (int elementID : elements){
			std::shared_ptr<Element> e = ElementTypes::GetElementPtr(ElementTypes::ElemntType(elementID));
			
			n.elements.push_back(e);
		}
		
		n.elements.push_back(ElementTypes::GetElementPtr(ElementTypes::TextField));

		return n;
	}
};

namespace Tabs { // managing note types - theres only one of these...
	class NoteManagerTap : public TiledTab {
	public:
		static std::unordered_map<std::string, NoteData> allNotes;
		static std::string editingElement;
		NoteManagerTap();
	};
}
