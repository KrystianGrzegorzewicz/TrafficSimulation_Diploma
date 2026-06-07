#pragma once

#include "vehicles/CarState.h"
#include "perception/PerceptionState.h"

class ILongitudinalModel
{
public:
	virtual ~ILongitudinalModel() = default;

	virtual float computeAcceleration(
		const CarState& self,
		const PerceptionState& perception,
		float desiredSpeed,
		float maxAccel,
		float maxDecel
	) = 0;
};