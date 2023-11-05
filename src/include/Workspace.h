#pragma once

#include <string>

class Workspace {
private:
	std::string myUUID;
	std::string prevUUID;

public:
	virtual void Draw(int x, int y, int w, int h) = 0;
	virtual void MousePressed(int x, int y) {};
	virtual void Update() {};
};