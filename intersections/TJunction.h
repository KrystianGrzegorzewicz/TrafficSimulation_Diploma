#pragma once
#pragma once
#include "core/Road.h"
#include "core/Block.h"
#include "core/Travel.h"

class TJunction : public Road {
private:
    std::vector<Block> blocks;
	std::vector<Travel> travels;
public:
    TJunction(float centerX, float centerY);
    TJunction();
    std::vector<Block> getBlocks();
	Travel getRandomTravel();
};