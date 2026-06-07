#pragma once

#include "road/Block.h"
#include "road/Travel.h"
#include "road/Line.h"
#include "road/Circle.h"

#include <vector>

class Junction
{
public:
	Junction();
	explicit Junction(int index);

	std::vector<Block>& getBlocks();
	const std::vector<Block>& getBlocks() const;
	std::vector<Line> getLines() const;
	std::vector<Circle> getCircles() const;
	std::vector<Circle> drawTravel(std::vector<Vec2> travelPoints);
	Travel getRandomTravel();
	const Block* getPerceptionMaskForTravel(int travelId, const Vec2& point) const;

private:
	std::vector<Block> blocks;
	std::vector<Travel> travels;
	std::vector<Line> lines;
	std::vector<Circle> circles;
};