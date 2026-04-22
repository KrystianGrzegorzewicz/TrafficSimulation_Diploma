#include "TJunction.h"
#include <random>
#include <iostream>

TJunction::TJunction() {
    blocks.push_back(Block(-100.0f, -4.0f, 100.0f, -3.5f));
	blocks.push_back(Block(-100.0f, 3.5f, -3.5f, 4.0f));
    blocks.push_back(Block(3.5f, 3.5f, 100.0f, 4.0f));
    blocks.push_back(Block(-4.0f, 3.5f, -3.5f, 50.0f));
    blocks.push_back(Block(3.5f, 3.5f, 4.0f, 50.0f));
	std::vector<Vec2> travelPoints1 = {
        Vec2(-90.0f, 1.75f),  // buffer start
        Vec2(-90.0f, 1.75f),
        Vec2(-5.0f, 1.75f),
        Vec2(5.0f, 1.75f),
        Vec2(90.0f, 1.75f),
        Vec2(90.0f, 1.75f)    // buffer end
    };
	travels.push_back(Travel(travelPoints1));
    std::vector<Vec2> travelPoints2 = {
        Vec2(-90.0f, 1.75f),  // buffer start
        Vec2(-90.0f, 1.75f),

        Vec2(-5.0f, 1.75f),
        Vec2(0.0f, 0.0f),
        Vec2(-1.75f, 5.0f),

        Vec2(-1.75f, 45.0f),
        Vec2(-1.75f, 45.0f)   // buffer end
    };
    travels.push_back(Travel(travelPoints2));
}
std::vector<Block> TJunction::getBlocks() {
    return blocks;
}
Travel TJunction::getRandomTravel() {
	int index = rand() % travels.size();
	return travels[index];
}