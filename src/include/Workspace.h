#pragma once

#include <string>

class Workspace {
private:
	std::string myUUID;
	std::string prevUUID;

	// a local edit mode that has a get fundtion
	// that returns the global edit mode
	// if the settings say we are currently using global edit mode.

public:
	virtual void Draw(int x, int y, int w, int h) = 0;
	virtual void MousePressed(int x, int y) {};
	virtual void Update() {};
};