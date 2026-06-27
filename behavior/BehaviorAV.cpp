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
	PathPlan plan = planner.compute(travel, segment, t, speed, 7.5f);

	cmd.targetPoint = plan.targetPoint;
	float desiredSpeed = std::min(maxSpeed, plan.maxCurveSpeed);

	// Zwalnianie przed fizyczną przeszkodą
	if (perception.hasBlockHazard && perception.hazardIsActive)
	{
		float hazardFactor = std::clamp(perception.hazardDistance / 45.f, 0.f, 1.f);
		desiredSpeed *= hazardFactor;
	}

	// Ustępowanie pierwszeństwa na punktach konfliktu (ronda, lewoskręty)
	if (perception.hasConflict)
	{
		// Zaczynamy zwalniać 30m przed punktem, dążąc do zera 6m przed nim
		float conflictFactor = std::clamp((perception.conflictDistance - 6.0f) / 24.0f, 0.0f, 1.0f);
		desiredSpeed *= conflictFactor;
	}

	cmd.longitudinalAcceleration =
		longitudinalModel
		->computeAcceleration(
			self,
			perception,
			desiredSpeed,
			maxAccel,
			maxDecel);

	// Hamowanie awaryjne w skrajnych przypadkach
	if (perception.hasBlockHazard && perception.hazardDistance < 6.0f)
		cmd.emergencyBrake = true;

	if (perception.hasConflict && perception.conflictDistance < 4.0f)
		cmd.emergencyBrake = true;

	if (perception.distanceToCarAhead < 6.0f)
		cmd.emergencyBrake = true;

	cmd.desiredSpeed = desiredSpeed;

	return cmd;
}