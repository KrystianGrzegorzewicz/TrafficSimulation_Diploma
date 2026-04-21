#pragma once
#include "core/Vec2.h"
#include <vector>

class Travel {
private:
	
public:
	Travel(std::vector<Vec2> points);
	std::vector<Vec2> TravelPoints;
};