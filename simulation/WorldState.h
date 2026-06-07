#pragma once

#include "road/Junction.h"
#include "vehicles/CarState.h"

#include <vector>

struct WorldState
{
	std::vector<CarState> vehicleStates;
	const Junction* junction = nullptr;
	float currentTime = 0.0f;
};