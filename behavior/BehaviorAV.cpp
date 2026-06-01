#include "behavior/BehaviorAV.h"
#include <algorithm>

BehaviorAV::BehaviorAV(std::unique_ptr<ILongitudinalModel> model)
	: longitudinalModel(std::move(model)) {
}

MotionCommand BehaviorAV::compute(
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
	MotionCommand cmd;

	float speed =
		self.velocity.length();

	PathPlan plan =
		planner.compute(
			travel,
			segment,
			t,
			speed,
			7.5f);

	cmd.targetPoint =
		plan.targetPoint;

	float desiredSpeed =
		std::min(
			maxSpeed,
			plan.maxCurveSpeed);

	// Predictive hazard braking
	if (perception.hasBlockHazard &&
		perception.hazardIsActive)
	{
		float hazardFactor =
			std::clamp(
				perception.hazardDistance / 45.f,
				0.f,
				1.f);

		desiredSpeed *= hazardFactor;
	}

	cmd.longitudinalAcceleration =
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
		cmd.emergencyBrake = true;
	}
	if (perception.distanceToCarAhead < 6.0f)
	{
		cmd.emergencyBrake = true;
	}

	return cmd;
}