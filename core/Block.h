#pragma once
#include "core/Vec2.h"

class Block {
private:
     Vec2 x0y0, xnyn;
	 int color[3];
public:
	Block(float x0, float y0, float xn, float yn);
	void getVisualization();
	int getColor(int index);
	Vec2 getTopLeft() const;
	Vec2 getBottomRight() const;
};