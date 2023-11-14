#pragma once

#include "Tab.h"
#include "Tile.h"

#include <string>
#include <vector>

#include <memory>

struct TilePair{
public:
	std::shared_ptr<Tile> tile;
	std::shared_ptr<TilePair> tp1;
	std::shared_ptr<TilePair> tp2;

	TilePair(std::shared_ptr<Tile> tile = std::make_shared<Tile>()) : tile{tile} {}

	// directions:
	//			  0: up
	//			  1: down
	//			  2: left
	//			  3: right
	void split(bool _horizontal, float _parting, std::shared_ptr<Tile> newTile){
		if (isAPair){
			return;
		}

		tp1 = { std::make_shared<TilePair>(tile) };
		tp2 = { std::make_shared<TilePair>(newTile) };

		tile = nullptr;

		horizontal = _horizontal;
		parting = _parting;
		isAPair = true;
	}

	bool isAPair = false;
	float parting = 0.5; // 0 to 1
	bool horizontal = true; // false = vertical | direction of parting

	void Draw(int x, int y, int w, int h){
		if (isAPair){
			if (horizontal){
				tp1->Draw(x, y, w * parting, h);
				tp2->Draw(x + w*parting, y, w * (1-parting), h);
			} else {
				tp1->Draw(x, y, w, h * parting);
				tp2->Draw(x, y + h*parting, w, h * (1-parting));
			}
		} else {
			BeginScissorMode(x, y, w, h);

			tile->Draw(x, y, w, h);

			EndScissorMode();
		}
	}

	void Update(){
		if (isAPair){
			tp1->Update();
			tp2->Update();
		} else {
			tile->Update();
		}
	}

	void MousePressed(int x, int y, int w, int h, int mx, int my){
		if (!(x < mx && x + w > mx && y < my && y + h > my))
			return;

		if (isAPair){
			if (horizontal){
				tp1->MousePressed(x, y, w * parting, h, mx, my);
				tp2->MousePressed(x + w*parting, y, w * (1-parting), h, mx, my);
			} else {
				tp1->MousePressed(x, y, w, h * parting, mx, my);
				tp2->MousePressed(x, y + h*parting, w, h * (1-parting), mx, my);
			}
		} else {
			tile->MousePressed(mx - x, my - y);
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
	void Update() override {
		tile.Update();
	};
	void MousePressed(int x, int y) override {
		tile.MousePressed(lastX, lastY, lastW, lastH, x, y);
	}
};