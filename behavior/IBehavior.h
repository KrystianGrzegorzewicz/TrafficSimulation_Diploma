#pragma once
#include "behavior/BehaviorOutput.h"
#include "perception/PerceptionState.h"
#include "road/Travel.h"

// Strategy interface for longitudinal + target-point decisions.
class IBehavior
{
public:
	virtual ~IBehavior() = default;

	virtual BehaviorOutput compute(
		Travel& travel,
		int segment,
		float t,
		float currentSpeed,
		float maxSpeed,
		float maxAccel,
		float maxDecel,
		float lookaheadBase,
		float lookaheadSpeedFactor,
		const PerceptionState& perception
	) = 0;
};