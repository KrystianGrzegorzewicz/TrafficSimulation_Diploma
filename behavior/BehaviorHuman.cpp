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

static float calculateHumanConflictMargin(
	const DriverPersonality& personality,
	const ConflictParameters& params)
{
	const float aggression =
		std::clamp(
			personality.aggression,
			0.0f,
			1.0f
		);

	const float aggressionEffect =
		std::pow(aggression, 1.5f);

	const float caution =
		1.0f - aggressionEffect;

	return
		params.humanBaseMargin +
		caution * params.humanAggressionMargin;
}

void BehaviorHuman::evaluateConflict(
	const CarState& self,
	float maxDecel,
	float& desiredSpeed,
	const PerceptionState& perception,
	MotionCommand& cmd,
	ConflictParameters& params,
	float dt)
{
	constexpr float INF = 999999.0f;

	if (conflictYieldTimer > 0.0f)
	{
		conflictYieldTimer -= dt;

		if (conflictYieldTimer < 0.0f)
			conflictYieldTimer = 0.0f;
	}

	if (!perception.hasConflict ||
		perception.alreadyEnteringConflict ||
		perception.priorityCarsTTA.empty())
	{
		if (yieldingToConflict)
		{
			const float resumeMargin =
				0.75f;

			const float myEntry =
				perception.selfTtaEntry;

			const float otherExit =
				perception.otherArrival;

			const bool enoughSpace =
				myEntry > otherExit +
				resumeMargin;

			if (conflictYieldTimer <= 0.0f &&
				enoughSpace)
			{
				yieldingToConflict = false;
				yieldingToCarId = -1;
			}
		}

		if (yieldingToConflict)
		{
			desiredSpeed =
				std::min(
					desiredSpeed,
					0.5f
				);
		}

		return;
	}

	const PriorityCarTTA& target =
		perception.priorityCarsTTA.front();

	const float margin =
		calculateHumanConflictMargin(
			personality,
			params
		);

	const bool temporalOverlap =
		perception.selfTtaEntry <
		target.ttaExit &&
		target.ttaEntry <
		perception.selfTtaExit;

	const float arrivalDifference =
		std::fabs(
			perception.selfTtaEntry -
			target.ttaEntry
		);

	const bool unsafe =
		temporalOverlap &&
		arrivalDifference < margin;

	if (!yieldingToConflict && unsafe)
	{
		yieldingToConflict = true;

		yieldingToCarId =
			target.carId;

		conflictYieldTimer =
			params.humanYieldHoldTime;
	}

	if (yieldingToConflict)
	{
		const float stopBuffer =
			params.humanStopBuffer;

		const float availableDistance =
			std::max(
				0.0f,
				perception.conflictDistance -
				stopBuffer
			);

		const float safeSpeed =
			std::sqrt(
				2.0f *
				maxDecel *
				availableDistance
			);

		desiredSpeed =
			std::min(
				desiredSpeed,
				safeSpeed
			);

		if (availableDistance < 3.0f)
		{
			desiredSpeed =
				std::min(
					desiredSpeed,
					1.0f
				);
		}

		if (availableDistance < 1.0f)
		{
			desiredSpeed = 0.0f;

			if (perception.conflictDistance < 2.5f)
			{
				cmd.emergencyBrake = true;
			}
		}

		return;
	}
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

	if (auto* idm =
		dynamic_cast<IDMLongitudinalModel*>(
			longitudinalModel.get()))
	{
		idm->setDriverFactors(
			personality.gapFactor,
			personality.accelFactor
		);
	}

	constexpr float dt = 0.01f;

	for (auto& item : reactionQueue)
		item.timeRemaining -= dt;

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

	const float speed =
		self.velocity.length();

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

	PathPlan plan =
		planner.compute(
			travel,
			segment,
			t,
			speed,
			6.0f
		);

	cmd.targetPoint =
		plan.targetPoint;

	float desiredSpeed =
		std::min(
			maxSpeed,
			plan.maxCurveSpeed *
			personality.curveFactor
		);

	const float overspeed =
		speed - desiredSpeed;

	if (overspeed > 0.0f)
	{
		desiredSpeed -=
			overspeed * 0.55f;
	}

	if (perception.hasBlockHazard &&
		perception.hazardIsActive)
	{
		const float hazardFactor =
			std::clamp(
				perception.hazardDistance /
				50.0f,
				0.0f,
				1.0f
			);

		desiredSpeed *=
			hazardFactor;
	}

	if (perception.hasConflict &&
		!perception.alreadyEnteringConflict &&
		!perception.priorityCarsTTA.empty())
	{
		const PriorityCarTTA& target =
			perception.priorityCarsTTA.front();

		const float safetyGap =
			1.0f +
			personality.gapFactor;

		const bool canCrossBefore =
			perception.selfTtaExit +
			safetyGap <
			target.ttaEntry;

		const bool canCrossAfter =
			target.ttaExit +
			safetyGap <
			perception.selfTtaEntry;

		if (!canCrossBefore &&
			!canCrossAfter)
		{
			const float stopBuffer =
				std::max(
					4.0f *
					personality.gapFactor,
					2.0f
				);

			const float availableDistance =
				std::max(
					0.0f,
					perception.conflictDistance -
					stopBuffer
				);

			const float safeSpeed =
				std::sqrt(
					2.0f *
					maxDecel *
					availableDistance
				);

			desiredSpeed =
				std::min(
					desiredSpeed,
					safeSpeed
				);

			if (availableDistance < 1.0f)
				desiredSpeed = 0.0f;

			if (perception.conflictDistance <
				stopBuffer + 1.5f)
			{
				cmd.emergencyBrake = true;
			}
		}
	}

	cmd.desiredSpeed =
		std::max(
			0.0f,
			desiredSpeed
		);

	const float desiredAccel =
		longitudinalModel->computeAcceleration(
			self,
			perception,
			cmd.desiredSpeed,
			maxAccel,
			maxDecel
		);

	reactionQueue.push_back(
		{
			desiredAccel,
			reactionTime
		}
	);

	if (haveDelayedCommand)
	{
		cmd.longitudinalAcceleration =
			delayedAccel;
	}
	else
	{
		cmd.longitudinalAcceleration = 0.0f;
	}

	if (startDelayTimer > 0.0f)
	{
		startDelayTimer -= dt;

		cmd.longitudinalAcceleration =
			0.0f;
	}

	if (perception.hasBlockHazard &&
		perception.hazardDistance <
		10.0f * personality.gapFactor)
	{
		cmd.emergencyBrake = true;
	}

	if (perception.hasConflict &&
		perception.conflictDistance <
		5.0f * personality.gapFactor)
	{
		cmd.emergencyBrake = true;
	}

	if (perception.hasCarAhead &&
		perception.distanceToCarAhead <
		10.0f * personality.gapFactor)
	{
		cmd.emergencyBrake = true;
	}

	return cmd;
}