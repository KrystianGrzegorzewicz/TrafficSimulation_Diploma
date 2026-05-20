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

	float speed = self.velocity.length();
	PathPlan plan = planner.compute(travel, segment, t, speed, 6.0f);

	newCmd.targetPoint = plan.targetPoint;

	float currentSpeed =
		self.velocity.length();

	float desiredSpeed =
		std::min(
			maxSpeed,
			plan.maxCurveSpeed);

	// anticipatory braking
	float overspeed =
		currentSpeed -
		desiredSpeed;

	if (overspeed > 0.f)
	{
		desiredSpeed -=
			overspeed * 0.35f;
	}

	if (perception.hasBlockHazard && perception.hazardIsActive)
	{
		if (perception.hazardDistance < 10) desiredSpeed = 0;
		else desiredSpeed *= 0.5f;
	}

	newCmd.longitudinalAcceleration =
		longitudinalModel->computeAcceleration(
			self, perception, desiredSpeed, maxAccel, maxDecel);

	if (perception.hasBlockHazard && perception.hazardDistance < 5)
		newCmd.emergencyBrake = true;

	// -------------------------------------------------
	// HUMAN REACTION DELAY
	// -------------------------------------------------

	// Dodajemy nową decyzję do bufora
	reactionBuffer.push_back(newCmd);

	// Ile kroków opóźnienia potrzebujemy?
	float dtEstimate = 0.016f; // sym step ~60Hz (możesz potem podać z zewnątrz)
	int stepsDelay = std::max(1, (int)(reactionTime / dtEstimate));

	// Jeśli bufor jeszcze nie pełny → człowiek "nie zdążył zareagować"
	if ((int)reactionBuffer.size() <= stepsDelay)
	{
		MotionCommand idle;
		idle.targetPoint = plan.targetPoint; // patrzy na drogę
		idle.longitudinalAcceleration = 0.0f; // ale jeszcze nic nie robi
		return idle;
	}

	// Zwracamy starą decyzję
	MotionCommand delayedCmd = reactionBuffer.front();
	reactionBuffer.pop_front();

	return delayedCmd;
}