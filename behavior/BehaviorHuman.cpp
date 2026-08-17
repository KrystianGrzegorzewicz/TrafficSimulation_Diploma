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

	/*
	 * --------------------------------------------------------
	 * Opóźnienie reakcji człowieka
	 * --------------------------------------------------------
	 */

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

	/*
	 * --------------------------------------------------------
	 * Opóźnienie ruszania
	 * --------------------------------------------------------
	 */

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

	/*
	 * --------------------------------------------------------
	 * Planowanie trasy
	 * --------------------------------------------------------
	 */

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

	/*
	 * Kierowca reaguje na przekroczenie
	 * prędkości wynikającej z geometrii trasy.
	 */
	const float overspeed =
		speed - desiredSpeed;

	if (overspeed > 0.0f)
	{
		desiredSpeed -=
			overspeed * 0.55f;
	}

	/*
	 * --------------------------------------------------------
	 * Przeszkody
	 * --------------------------------------------------------
	 */

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

	/*
	 * --------------------------------------------------------
	 * PUNKTY KONFLIKTOWE
	 * --------------------------------------------------------
	 */

	if (perception.hasConflict &&
		!perception.alreadyEnteringConflict &&
		!perception.priorityCarsTTA.empty())
	{
		const PriorityCarTTA& target =
			perception.priorityCarsTTA.front();

		/*
		 * Indywidualny margines kierowcy.
		 */
		const float safetyGap =
			1.0f +
			personality.gapFactor;

		/*
		 * Okno czasowe:
		 *
		 * 1. self exit < other entry
		 * 2. other exit < self entry
		 */
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

			/*
			 * Prędkość pozwalająca zatrzymać się
			 * przed punktem konfliktowym.
			 */
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

	/*
	 * --------------------------------------------------------
	 * Model wzdłużny
	 * --------------------------------------------------------
	 */

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

	/*
	 * Kierowca nie wykonuje decyzji natychmiast.
	 */
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

	/*
	 * Opóźnienie ruszania.
	 */
	if (startDelayTimer > 0.0f)
	{
		startDelayTimer -= dt;

		cmd.longitudinalAcceleration =
			0.0f;
	}

	/*
	 * --------------------------------------------------------
	 * Awaryjne hamowanie
	 * --------------------------------------------------------
	 */

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