#pragma once
#include "road/Block.h"
#include "road/Travel.h"
#include "road/Line.h"
#include "road/Circle.h"
#include <vector>

// Renamed from TJunction.  API is identical so Simulation and
// WorldState only need an include-path update.
class Junction
{
public:
    Junction();
    explicit Junction(int index);

    // Mutable so Block::update() can be called each tick
    std::vector<Block>& getBlocks();
    const std::vector<Block>& getBlocks() const;
    std::vector<Line>          getLines()   const;
    std::vector<Circle>        getCircles() const;
    Travel                     getRandomTravel();

private:
    std::vector<Block>  blocks;
    std::vector<Travel> travels;
    std::vector<Line>   lines;
    std::vector<Circle> circles;
};