#include "behavior/BehaviorHuman.h"
#include <algorithm>

BehaviorHuman::BehaviorHuman(std::unique_ptr<ILongitudinalModel> model)
	: longitudinalModel(std::move(model)) {
}

MotionCommand BehaviorHuman::compute(
	Travel& travel,
	int segment,
	float t,
	const CarState& self,
	float maxSpeed,
	float maxAccel,
	float maxDecel,
	float,
	float,
	const PerceptionState& perception)
{
	MotionCommand newCmd;

	float speed =
		self.velocity.length();

	PathPlan plan =
		planner.compute(
			travel,
			segment,
			t,
			speed,
			6.0f);

	newCmd.targetPoint =
		plan.targetPoint;

	float desiredSpeed =
		std::min(
			maxSpeed,
			plan.maxCurveSpeed);

	// Human anticipatory braking
	float overspeed =
		speed -
		desiredSpeed;

	if (overspeed > 0.f)
	{
		desiredSpeed -=
			overspeed * 0.55f;
	}

	// Hazard anticipation
	if (perception.hasBlockHazard &&
		perception.hazardIsActive)
	{
		float hazardFactor =
			std::clamp(
				perception.hazardDistance / 50.f,
				0.f,
				1.f);

		desiredSpeed *= hazardFactor;
	}

	newCmd.longitudinalAcceleration =
		longitudinalModel
		->computeAcceleration(
			self,
			perception,
			desiredSpeed,
			maxAccel,
			maxDecel);

	if (perception.hasBlockHazard &&
		perception.hazardDistance < 6.0f)
	{
		newCmd.emergencyBrake = true;
	}

	if (perception.distanceToCarAhead < 6.0f)
	{
		newCmd.emergencyBrake = true;
	}

	// Natural human delay
	reactionBuffer.push_back(newCmd);

	const int stepsDelay = 12;

	if ((int)reactionBuffer.size()
		<= stepsDelay)
	{
		MotionCommand idle;
		idle.targetPoint =
			plan.targetPoint;

		return idle;
	}

	MotionCommand delayed =
		reactionBuffer.front();

	reactionBuffer.pop_front();

	return delayed;
}