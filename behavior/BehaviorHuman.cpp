#include "behavior/BehaviorHuman.h"
#include "behavior/longitudinal/IDMLongitudinalModel.h"

#include <algorithm>

BehaviorHuman::BehaviorHuman(
	std::unique_ptr<ILongitudinalModel> model,
	const DriverPersonality& personality)
	: longitudinalModel(std::move(model))
	, personality(personality)
{
	reactionTime =
		0.8f *
		personality.reactionFactor;
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

	//----------------------------------------------------
	// Driver-specific IDM tuning
	//----------------------------------------------------

	if (auto* idm =
		dynamic_cast<IDMLongitudinalModel*>(
			longitudinalModel.get()))
	{
		idm->setDriverFactors(
			personality.gapFactor,
			personality.accelFactor);
	}

	//----------------------------------------------------
	// Update reaction queue timers
	//----------------------------------------------------

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
			delayedAccel =
				reactionQueue.front().accel;

			reactionQueue.pop_front();

			haveDelayedCommand = true;
		}
		else
		{
			break;
		}
	}

	//----------------------------------------------------
	// Current speed
	//----------------------------------------------------

	float speed =
		self.velocity.length();

	//----------------------------------------------------
	// Detect stop -> apply start delay
	//----------------------------------------------------

	if (speed < 0.2f)
	{
		if (!wasStopped)
		{
			wasStopped = true;

			startDelayTimer =
				personality.startDelay;
		}
	}
	else
	{
		wasStopped = false;
	}

	//----------------------------------------------------
	// Path planning
	//----------------------------------------------------

	PathPlan plan =
		planner.compute(
			travel,
			segment,
			t,
			speed,
			6.0f);

	cmd.targetPoint =
		plan.targetPoint;

	//----------------------------------------------------
	// Curve speed anticipation
	//----------------------------------------------------

	float desiredSpeed =
		std::min(
			maxSpeed,
			plan.maxCurveSpeed *
			personality.curveFactor);

	float overspeed =
		speed - desiredSpeed;

	if (overspeed > 0.0f)
	{
		desiredSpeed -=
			overspeed * 0.55f;
	}

	//----------------------------------------------------
	// Hazard anticipation
	//----------------------------------------------------

	if (perception.hasBlockHazard &&
		perception.hazardIsActive)
	{
		float hazardFactor =
			std::clamp(
				perception.hazardDistance / 50.0f,
				0.0f,
				1.0f);

		desiredSpeed *= hazardFactor;
	}

	//----------------------------------------------------
	// Compute desired acceleration
	//----------------------------------------------------

	float desiredAccel =
		longitudinalModel->computeAcceleration(
			self,
			perception,
			desiredSpeed,
			maxAccel,
			maxDecel);

	//----------------------------------------------------
	// Store command in reaction queue
	//----------------------------------------------------

	reactionQueue.push_back(
		{
			desiredAccel,
			reactionTime
		});

	//----------------------------------------------------
	// Apply delayed command
	//----------------------------------------------------

	if (haveDelayedCommand)
	{
		cmd.longitudinalAcceleration =
			delayedAccel;
	}
	else
	{
		cmd.longitudinalAcceleration =
			0.0f;
	}

	//----------------------------------------------------
	// Human restart delay
	//----------------------------------------------------

	if (startDelayTimer > 0.0f)
	{
		startDelayTimer -= dt;

		cmd.longitudinalAcceleration =
			0.0f;
	}

	//----------------------------------------------------
	// Emergency braking
	//----------------------------------------------------

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