#pragma once
#pragma once
#include "core/Block.h"
#include "core/Travel.h"

class TJunction {
private:
    std::vector<Block> blocks;
	std::vector<Travel> travels;
public:
    TJunction();
    TJunction(int index);
    std::vector<Block> getBlocks();
	Travel getRandomTravel();
};