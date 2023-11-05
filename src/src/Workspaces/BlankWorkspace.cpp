#include "../../include/Workspaces/BlankWorkspace.h"

void Workspaces::BlankWorkspace::Draw(int x, int y, int w, int h) {
	// scissor mode
	BeginScissorMode(x, y, w, h);

	DrawRectangle(0, 0, 2000, 2000, BLUE);

	for (NoteWithPosition NWP : notes) {
		NWP.note.get()->Draw();
	}

	EndScissorMode();
}

void Workspaces::BlankWorkspace::MousePressed(int x, int y) {
	for (NoteWithPosition NWP : notes) {
		Vector2 position = NWP.position;
		Vector2 size = NWP.note.get()->size;

		if (x > position.x && x < position.x + size.x && y > position.y && y < position.y + size.y) {
			NWP.note.get()->MousePressed({ x - position.x, y - position.y });
		}
	}
}

void Workspaces::BlankWorkspace::Update() {}