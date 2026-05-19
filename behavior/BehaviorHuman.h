// behavior/BehaviorHuman.h
#pragma once
#include "behavior/IBehavior.h"
#include "behavior/longitudinal/ILongitudinalModel.h"
#include "planning/PathPlanner.h"
#include <memory>
#include <deque>

class BehaviorHuman : public IBehavior
{
public:
	explicit BehaviorHuman(std::unique_ptr<ILongitudinalModel> longitudinalModel);

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

	std::deque<MotionCommand> reactionBuffer;

	float reactionTime = 0.8f;      // sekundy (tuning)
	float accumulatedTime = 0.0f;
};