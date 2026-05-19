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

	float speed = self.velocity.length();
	PathPlan plan = planner.compute(travel, segment, t, speed, 6.0f);

	cmd.targetPoint = plan.targetPoint;

	float desiredSpeed = std::min(maxSpeed, plan.maxCurveSpeed);

	if (perception.hasBlockHazard && perception.hazardIsActive)
	{
		if (perception.hazardDistance < 10) desiredSpeed = 0;
		else desiredSpeed *= 0.5f;
	}

	cmd.longitudinalAcceleration = longitudinalModel->computeAcceleration(
		self, perception, desiredSpeed, maxAccel, maxDecel);

	if (perception.hasBlockHazard && perception.hazardDistance < 5)
		cmd.emergencyBrake = true;

	return cmd;
}