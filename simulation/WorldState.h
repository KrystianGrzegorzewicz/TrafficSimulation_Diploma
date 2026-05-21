#pragma once
#include "road/Junction.h"
//#include "vehicles/Car.h"   // forward-declared for CarState
#include <vector>
#include "vehicles/CarState.h"
// Shared, simulation-wide read-only context.
// Passed by const reference into Perception and Behavior each tick.
struct WorldState
{
	// All current vehicle states (updated once per sim step before any update())
	std::vector<CarState> vehicleStates;

	// Road geometry (blocks, travels, etc.)
	// Perception reads blocks directly — BlockPerception concept removed.
	const Junction* junction = nullptr;

	float currentTime = 0.0f;
};