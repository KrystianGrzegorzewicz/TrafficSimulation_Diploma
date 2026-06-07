#pragma once

#include "behavior/MotionCommand.h"
#include "perception/PerceptionState.h"
#include "road/Travel.h"
#include "vehicles/CarState.h"

class IBehavior
{
public:
	virtual ~IBehavior() = default;

	virtual MotionCommand compute(
		Travel& travel,
		int segment,
		float t,
		const CarState& self,
		float maxSpeed,
		float maxAccel,
		float maxDecel,
		float lookaheadBase,
		float lookaheadSpeedFactor,
		const PerceptionState& perception
	) = 0;
};