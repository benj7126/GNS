#pragma once

#include "../Workspace.h"
#include "../Note.h"

#include "../Elements/SimpleTextField.h"


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

			testNote.note.get()->elements.push_back(std::make_shared<Elements::SimpleTextField>());

			notes.push_back(testNote);
		}
	};
}