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

	const float speed =
		self.velocity.length();

	PathPlan plan =
		planner.compute(
			travel,
			segment,
			t,
			speed,
			7.5f
		);

	cmd.targetPoint =
		plan.targetPoint;

	float desiredSpeed =
		std::min(
			maxSpeed,
			plan.maxCurveSpeed
		);

	if (perception.hasBlockHazard &&
		perception.hazardIsActive)
	{
		const float hazardFactor =
			std::clamp(
				perception.hazardDistance /
				45.0f,
				0.0f,
				1.0f
			);

		desiredSpeed *=
			hazardFactor;
	}

	evaluateConflictPoints(
		self,
		maxDecel,
		desiredSpeed,
		perception,
		cmd
	);

	desiredSpeed =
		std::max(
			0.0f,
			desiredSpeed
		);

	cmd.longitudinalAcceleration =
		longitudinalModel->computeAcceleration(
			self,
			perception,
			desiredSpeed,
			maxAccel,
			maxDecel
		);

	if (perception.hasBlockHazard &&
		perception.hazardDistance < 6.0f)
	{
		cmd.emergencyBrake = true;
	}

	if (perception.hasConflict &&
		!perception.alreadyEnteringConflict &&
		perception.conflictDistance < 3.0f)
	{
		cmd.emergencyBrake = true;
	}

	if (perception.hasCarAhead &&
		perception.distanceToCarAhead < 6.0f)
	{
		cmd.emergencyBrake = true;
	}

	cmd.desiredSpeed =
		desiredSpeed;

	return cmd;
}

void BehaviorAV::evaluateConflictPoints(
	const CarState& self,
	float maxDecel,
	float& desiredSpeed,
	const PerceptionState& perception,
	MotionCommand& cmd)
{
	if (!perception.hasConflict)
		return;

	if (perception.alreadyEnteringConflict)
		return;

	if (perception.priorityCarsTTA.empty())
		return;

	constexpr float kAVMargin = 0.8f;
	constexpr float kHumanExtraMargin = 1.0f;
	constexpr float kAVStopBuffer = 2.0f;
	constexpr float kHumanStopBuffer = 4.0f;

	for (const auto& candidate :
		perception.priorityCarsTTA)
	{
		if (candidate.ttaEntry >= 999998.f)
			continue;

		const float safetyMargin =
			candidate.isAV
			? kAVMargin
			: kAVMargin + kHumanExtraMargin;

		const float stopBuffer =
			candidate.isAV
			? kAVStopBuffer
			: kHumanStopBuffer;

		const float arrivalDifference =
			std::fabs(
				perception.selfTtaEntry -
				candidate.ttaEntry
			);

		if (arrivalDifference >= safetyMargin)
			continue;

		const bool temporalConflict =
			perception.selfTtaEntry <
			candidate.ttaExit &&
			candidate.ttaEntry <
			perception.selfTtaExit;

		if (!temporalConflict)
			continue;

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

		if (!candidate.isAV)
		{
			const float conservativeSpeed =
				std::sqrt(
					2.0f *
					maxDecel *
					std::max(
						0.0f,
						availableDistance - 1.0f
					)
				);

			desiredSpeed =
				std::min(
					desiredSpeed,
					conservativeSpeed
				);
		}

		if (availableDistance < 1.0f)
		{
			desiredSpeed = 0.0f;
		}

		if (perception.conflictDistance <
			stopBuffer + 1.0f)
		{
			cmd.emergencyBrake = true;
		}

		break;
	}
}