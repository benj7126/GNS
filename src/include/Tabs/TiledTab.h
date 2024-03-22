#pragma once

#include "Tab.h"
#include "Tile.h"

#include <string>
#include <vector>

#include <memory>

struct TilePair{
private:
	int grabbed = -1;

public:
	std::shared_ptr<Tile> tile;
	std::vector<std::shared_ptr<TilePair>> tps;

	TilePair(std::shared_ptr<Tile> tile = std::make_shared<Tile>()) : tile{tile} {}

	// directions:
	//			  0: up
	//			  1: down
	//			  2: left
	//			  3: right
	void split(bool _horizontal, std::vector<float> _partings, std::vector<std::shared_ptr<Tile>> tiles){
		if (isAPair || _partings.size()+1 != tiles.size()){
			std::cout << "either is not a pair or dosent have right amount of partings to tiles" << std::endl;
			return;
		}

		tile = nullptr;

		horizontal = _horizontal;
		partings = _partings;
		for (auto &tile : tiles)
			tps.push_back(std::make_shared<TilePair>(tile));

		isAPair = true;
	}

	bool isAPair = false;
	std::vector<float> partings;
	bool horizontal = true; // false = vertical | direction of parting

	void Draw(int x, int y, int w, int h){
		if (isAPair){
			float lastParting = 0;
			for (int i = 0; i < tps.size(); i++){
				float thisParting = 0;
				if (i == tps.size()-1){
					thisParting = 1 - lastParting;
					if (thisParting < 0)
						thisParting = 0;
				} else {
					thisParting = partings[i] - lastParting;
				}
				
				if (horizontal){
					tps[i]->Draw(x + lastParting * w, y, w * thisParting, h);
				} else {
					tps[i]->Draw(x, y + lastParting * h, w, h * thisParting);
				}
				lastParting += thisParting;
			}
		} else {
			BeginScissorMode(x, y, w, h);

			tile->Draw(x, y, w, h);

			EndScissorMode();
		}
	}

	void Update(int x, int y, int w, int h, int mx, int my){
		if (isAPair){
			if (grabbed != -1){
				if (IsMouseButtonReleased(0)){
					grabbed = -1;
				} else {
					float nParting = (float)mx / (float)w;
					if (horizontal)
						nParting = (float)mx / (float)w;
					else
						nParting = (float)my / (float)h;

					// instead of 0.2, make it dynamically calculate the minimum pixel size, and then make it fit.

					float prevParting = (grabbed-1 < 0 ? 0 : partings[grabbed-1]);
					if (nParting - prevParting < 0.2)
						nParting = prevParting + 0.2;

					float nextParting = (grabbed+2 > partings.size() ? 1 : partings[grabbed+1]);
					if (nextParting - nParting < 0.2)
						nParting = nextParting - 0.2;
					
					//std::cout << "g: " << grabbed << " - " << nParting << " | " << prevParting << " - " << nextParting << std::endl;

					partings[grabbed] = nParting;
				}
			}

			float lastParting = 0;
			for (int i = 0; i < tps.size(); i++){
				float thisParting = 0;
				if (i == tps.size()){
					thisParting = 1 - lastParting;
					if (thisParting < 0)
						thisParting = 0;
				} else {
					thisParting = partings[i] - lastParting;
				}
				if (horizontal){
					tps[i]->Update(x + lastParting * w, y, w * thisParting, h, mx, my);
				} else {
					tps[i]->Update(x, y + lastParting * h, w, h * thisParting, mx, my);
				}
				lastParting += partings[i];
			}
		} else {
			tile->Update();
		}
	}

	void MousePressed(int x, int y, int w, int h, int mx, int my){
		if (!(x < mx && x + w > mx && y < my && y + h > my))
			return;

		if (isAPair){
			float lastParting = 0;
			for (int i = 0; i < tps.size() - 1; i++){
				float thisParting = partings[i] - lastParting;

				if (horizontal){
					//std::cout << (x + (lastParting + thisParting) * w - mx) << " - " << (x + (lastParting + thisParting) * w - mx) << std::endl;
					//std::cout << lastParting << " - " << thisParting << std::endl;
					if (-10 < x + (lastParting + thisParting) * w - mx && x + (lastParting + thisParting) * w - mx < 10){
						grabbed = i;
						return;
					}

					tps[i] -> MousePressed(x + lastParting * w, y, w * thisParting, h, mx, my);
				} else {
					if (-10 < y + (lastParting + thisParting) * h - my && y + (lastParting + thisParting) * h - my < 10){
						grabbed = i;
						return;
					}
					
					tps[i] -> MousePressed(x, y + lastParting * h, w, h * thisParting, mx, my);
				}

				lastParting = thisParting;
			}
		} else {
			tile->MousePressed(mx - x, my - y);
		}
	}

	void UnGrab(){
		if (isAPair){
			grabbed = -1;
			for (int i = 0; i < tps.size(); i++)
				tps[i]->UnGrab();
		}
	}
};

class TiledTab : public Tab {
protected:
	TilePair tile;
	
	int lastX = 0;
	int lastY = 0;
	int lastW = 0;
	int lastH = 0;

public:
	void Draw(int x, int y, int w, int h) override {
		lastW = w;
		lastH = h;

		tile.Draw(x, y, w, h);
	};

	void Update(int x, int y) override {
		tile.Update(lastX, lastY, lastW, lastH, x, y);
	};

	void MousePressed(int x, int y) override {
		tile.UnGrab();
		tile.MousePressed(lastX, lastY, lastW, lastH, x, y);
	}
};
