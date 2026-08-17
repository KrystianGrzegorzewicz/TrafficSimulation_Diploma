#pragma once

#include "core/Vec2.h"

struct CarState
{
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 forward;
	int travelId = -1;
	int id = -1;
	bool isAV = false;
};