#pragma once

#include "perception/PerceptionState.h"
#include "simulation/WorldState.h"
#include "vehicles/CarState.h"

class IPerception
{
public:
	virtual ~IPerception() = default;

	virtual void update(
		const CarState& self,
		const WorldState& world,
		PerceptionState& outState
	) = 0;
};