#pragma once
#include "behavior/IBehavior.h"

// Human driver model.
// Longitudinal: Intelligent Driver Model (IDM).
// Block avoidance: three-zone braking (carried over, unified with perception).
// Future: Takagi–Sugeno fuzzy logic will replace or augment computeIDM().
//   Stub hooks are left as private virtual methods so fuzzy logic can be
//   dropped in without changing the public interface.
class BehaviorHuman : public IBehavior
{
public:
	BehaviorOutput compute(
		Travel& travel,
		int	segment,
		float t,
		float currentSpeed,
		float maxSpeed,
		float maxAccel,
		float maxDecel,
		float lookaheadBase,
		float lookaheadSpeedFactor,
		const PerceptionState& perception
	) override;

private:
	Vec2 computeTargetPoint(
		Travel& travel, int segment, float t, float lookahead
	);

	// IDM longitudinal acceleration
	float computeIDMAcceleration(
		float v, float v0,
		float maxAccel, float maxDecel,
		const PerceptionState& perception
	);

	// Block braking — was computeBlockAvoidanceDeceleration in old BehaviorModel
	float computeBlockBraking(
		float currentSpeed,
		float maxDecel,
		const PerceptionState& perception
	);

	// --- Takagi–Sugeno fuzzy hook (stub, returns IDM value for now) ---
	// Replace body with fuzzy inference when ready.
	virtual float fuzzyLongitudinalOutput(
		float v, float v0, float gap, float dv
	);
};