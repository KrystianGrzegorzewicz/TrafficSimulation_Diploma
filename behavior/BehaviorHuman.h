#pragma once

#include "behavior/IBehavior.h"
#include "behavior/longitudinal/ILongitudinalModel.h"
#include "planning/PathPlanner.h"
#include "vehicles/DriverPersonality.h"

#include <memory>
#include <deque>

class BehaviorHuman : public IBehavior
{
public:
	explicit BehaviorHuman(
		std::unique_ptr<ILongitudinalModel> longitudinalModel,
		const DriverPersonality& personality);

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
	DriverPersonality personality;

private:
	std::unique_ptr<ILongitudinalModel> longitudinalModel;
	PathPlanner planner;

	struct DelayedAccel
	{
		float accel;
		float timeRemaining;
	};

	std::deque<DelayedAccel> reactionQueue;

	float reactionTime = 0.4f;      // sekundy (tuning)
	float accumulatedTime = 0.0f;
	bool wasStopped = false;
	float startDelayTimer = 0.0f;
};