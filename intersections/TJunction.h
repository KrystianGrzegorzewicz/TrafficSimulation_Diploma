#pragma once
#pragma once
#include "core/Block.h"
#include "core/Travel.h"
#include "core/Line.h"
#include "core/Circle.h"
#include <vector>

class TJunction {
private:
    std::vector<Block> blocks;
	std::vector<Travel> travels;
    std::vector<Line> lines;
    std::vector<Circle> circles;
    
public:
    TJunction();
    TJunction(int index);
    std::vector<Block> getBlocks();
    std::vector<Line> getLines();
    std::vector<Circle> getCircles();
	Travel getRandomTravel();
};
