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
			NoteWithPosition testNote{ std::make_shared<Note>("a", Vector2{250, 80}), {0, 0} };

			std::shared_ptr<Elements::TextField> TF = std::make_shared<Elements::TextField>();
			
			TF->size = {250, 80};

			testNote.note.get()->elements.push_back(TF);

			notes.push_back(testNote);
			
			
			NoteWithPosition testNote2{ std::make_shared<Note>("b", Vector2{250, 80}), {300, 0} };

			std::shared_ptr<Elements::TextField> TF2 = std::make_shared<Elements::TextField>();
			
			TF2->size = {250, 80};
			TF2->wrapping = 1;

			testNote2.note.get()->elements.push_back(TF2);

			notes.push_back(testNote2);
			
			
			NoteWithPosition testNote3{ std::make_shared<Note>("c", Vector2{250, 80}), {600, 0} };

			std::shared_ptr<Elements::TextField> TF3 = std::make_shared<Elements::TextField>();
			
			TF3->size = {250, 80};
			TF3->wrapping = 2;

			testNote3.note.get()->elements.push_back(TF3);

			notes.push_back(testNote3);
		}
	};
}
