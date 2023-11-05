#pragma once

#include "Tab.h"
#include <vector>
#include <memory>

class ApplicationManager {
private:
	int curTap = 0;
	int lockedTapCount;
	std::vector<std::shared_ptr<Tab>> tabs{};

public:
	ApplicationManager();

	int W;
	int H;

	void AddWorkspace();
	
	void Draw();
	void Update();
};