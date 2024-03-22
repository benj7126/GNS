#include "ShowWindowTile.h"

#include "NoteManagerTap.h"

#include "ElementType.h"

#include <raylib.h>

namespace Tabs::Tiles{
	void ShowWindowTile::Draw(int x, int y, int w, int h){
		DrawRectangleLines(x, y, w, 20, BLACK);
		noteName.size = {float(w), 20};
		noteName.Draw({float(x), float(y)});
		y += 20;
		h -= 20;


		DrawRectangle(x, y, w, h, WHITE);
		DrawRectangleLines(x, y, w, h, BLACK);
		
		// std::cout << Tabs::NoteManagerTap::editingElement << std::endl;
		// fake press new button.

		if (NoteManagerTap::allNotes.find(NoteManagerTap::editingElement) == NoteManagerTap::allNotes.end()){
			NoteData n = {};

			n.size = {200, 240};

			// n.elements.push_back

			NoteManagerTap::allNotes.insert({NoteManagerTap::editingElement, n});
		}

		Note n = NoteManagerTap::allNotes[NoteManagerTap::editingElement].genNote();
		n.Draw({float(x) + w/2 - n.size.x/2, float(y) + h/2 - n.size.y/2});

	}

	void ShowWindowTile::Update() {
		noteName.Update();
	}

	void ShowWindowTile::MousePressed(int x, int y){
		if (y < noteName.size.y)
			noteName.MousePressed({float(x), float(y)});

		std::cout << Tabs::NoteManagerTap::editingElement << std::endl;
	}
}
