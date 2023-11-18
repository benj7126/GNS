#pragma once

#include "Workspace.h"
#include "Note.h"
#include "Elements/TextField.h"

#include <raylib.h>
#include <vector>


namespace Workspaces {
	class BlankWorkspace : public Workspace {
	protected:
		struct NoteWithPosition {
			std::shared_ptr<Note> note;
			Vector2 position;
		};
		std::vector<NoteWithPosition> notes{};

	public:
		void Draw(int x, int y, int w, int h) override;
		void MousePressed(int x, int y) override;
		void Update() override;

		void TestAddNote() {
			NoteWithPosition testNote{ std::make_shared<Note>(), {0, 0} };

			testNote.note.get()->size = {250, 50};

			std::shared_ptr<Elements::TextField> TF = std::make_shared<Elements::TextField>();
			
			TF->size = {160, 45};

			testNote.note.get()->elements.push_back(TF);

			notes.push_back(testNote);
		}
	};
}