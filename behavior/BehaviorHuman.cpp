#include "behavior/BehaviorHuman.h"
#include "behavior/longitudinal/IDMLongitudinalModel.h"

#include <algorithm>

BehaviorHuman::BehaviorHuman(
	std::unique_ptr<ILongitudinalModel> model,
	const DriverPersonality& personality)
	: longitudinalModel(std::move(model))
	, personality(personality)
{
	reactionTime = 0.1f + 0.2f * personality.reactionFactor;
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
	MotionCommand cmd;

	if (auto* idm = dynamic_cast<IDMLongitudinalModel*>(longitudinalModel.get()))
	{
		idm->setDriverFactors(personality.gapFactor, personality.accelFactor);
	}

	constexpr float dt = 0.01f;

	for (auto& item : reactionQueue)
	{
		item.timeRemaining -= dt;
	}

	float delayedAccel = 0.0f;
	bool haveDelayedCommand = false;

	while (!reactionQueue.empty())
	{
		if (reactionQueue.front().timeRemaining <= 0.0f)
		{
			delayedAccel = reactionQueue.front().accel;
			reactionQueue.pop_front();
			haveDelayedCommand = true;
		}
		else
		{
			break;
		}
	}

	float speed = self.velocity.length();

	if (speed < 0.2f)
	{
		if (!wasStopped)
		{
			wasStopped = true;
			startDelayTimer = personality.startDelay;
		}
	}
	else
	{
		wasStopped = false;
	}

	PathPlan plan = planner.compute(travel, segment, t, speed, 6.0f);

	cmd.targetPoint = plan.targetPoint;
	float desiredSpeed = std::min(maxSpeed, plan.maxCurveSpeed * personality.curveFactor);

	float overspeed = speed - desiredSpeed;

	if (overspeed > 0.0f)
	{
		desiredSpeed -= overspeed * 0.55f;
	}

	if (perception.hasBlockHazard && perception.hazardIsActive)
	{
		float hazardFactor = std::clamp(perception.hazardDistance / 50.0f, 0.0f, 1.0f);
		desiredSpeed *= hazardFactor;
	}

	// Ustępowanie pierwszeństwa (ludzie)
	if (perception.hasConflict)
	{
		// Odległość zatrzymania jest modyfikowana przez charakter kierowcy
		float stopDist = std::max(6.0f * personality.gapFactor, 3.0f);
		float conflictFactor = std::clamp((perception.conflictDistance - stopDist) / 30.0f, 0.0f, 1.0f);
		desiredSpeed *= conflictFactor;
	}

	cmd.desiredSpeed = desiredSpeed;

	float desiredAccel =
		longitudinalModel->computeAcceleration(
			self,
			perception,
			desiredSpeed,
			maxAccel,
			maxDecel);

	reactionQueue.push_back({ desiredAccel,reactionTime });

	// Zastosowanie poleceń z opóźnieniem (czas reakcji człowieka)
	if (haveDelayedCommand)
	{
		cmd.longitudinalAcceleration = delayedAccel;
	}
	else
	{
		cmd.longitudinalAcceleration = 0.0f;
	}

	// Opóźnienie przy ruszaniu
	if (startDelayTimer > 0.0f)
	{
		startDelayTimer -= dt;
		cmd.longitudinalAcceleration = 0.0f;
	}

	// Hamowanie awaryjne
	if (perception.hasBlockHazard && perception.hazardDistance < 10.0f * personality.gapFactor)
	{
		cmd.emergencyBrake = true;
	}

	if (perception.hasConflict && perception.conflictDistance < 5.0f * personality.gapFactor)
	{
		cmd.emergencyBrake = true;
	}

	if (perception.distanceToCarAhead < 10.0f * personality.gapFactor)
	{
		cmd.emergencyBrake = true;
	}

	return cmd;
}