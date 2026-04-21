#include "core/Block.h"
#include <iostream>

Block::Block(float x0, float y0, float xn, float yn) : x0y0(x0, y0), xnyn(xn, yn) {
	Block::color[0] = 255;
	Block::color[1] = 255;
	Block::color[2] = 255;
}

void Block::getVisualization() {
	std::cout << "Block: (" << x0y0.x << ", " << x0y0.y << ") to (" << xnyn.x << ", " << xnyn.y << ") with color ("
		<< color[0] << ", " << color[1] << ", " << color[2] << ")" << std::endl;
}
Vec2 Block::getTopLeft() const {
	return x0y0;
}

Vec2 Block::getBottomRight() const {
	return xnyn;
}
/*int getColor(int index) {
	if (index < 0 || index > 2) {
		std::cerr << "Error: Color index out of range. Valid range is 0-2." << std::endl;
		return -1;
	}
	return color[index];
}*/