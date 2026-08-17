#pragma once

#include "behavior/IBehavior.h"
#include "behavior/longitudinal/ILongitudinalModel.h"
#include "planning/PathPlanner.h"

#include <memory>

class BehaviorAV : public IBehavior
{
public:
	explicit BehaviorAV(std::unique_ptr<ILongitudinalModel> longitudinalModel);

	MotionCommand compute(
		Travel& travel,
		int segment,
		float t,
		const CarState& self,
		float maxSpeed,
		float maxAccel,
		float maxDecel,
		float lookaheadBase,
		float lookaheadSpeedFactor,
		const PerceptionState& perception) override;

private:
	std::unique_ptr<ILongitudinalModel> longitudinalModel;
	PathPlanner planner;

	void evaluateConflictPoints(
		const CarState& self,
		float maxDecel,
		float& desiredSpeed,
		const PerceptionState& perception,
		MotionCommand& cmd
	);
};