#include "behavior/BehaviorHuman.h"
#include <algorithm>
#include <cmath>

// ---------------------------------------------------------------------------
// Public: compute()
// ---------------------------------------------------------------------------

BehaviorOutput BehaviorHuman::compute(
	Travel& travel,
	int                    segment,
	float                  t,
	float                  currentSpeed,
	float                  maxSpeed,
	float                  maxAccel,
	float                  maxDecel,
	float                  lookaheadBase,
	float                  lookaheadSpeedFactor,
	const PerceptionState& perception
)
{
	BehaviorOutput out;

	const auto& p = travel.TravelPoints;

	// Guard: need at least three control points ahead.
	if (segment + 2 >= static_cast<int>(p.size()))
	{
		out.acceleration = Vec2(0.f, 0.f);
		out.targetPoint = p.back();
		return out;
	}

	float lookahead = lookaheadBase + currentSpeed * lookaheadSpeedFactor;

	// ---- Target point for SteeringModel ----
	out.targetPoint = computeTargetPoint(travel, segment, t, lookahead);

	// ---- Curve-limited desired speed ----
	float curveSpeed =
		travel.computeSpeedLimitAhead(segment, t, lookahead, 6.0f);

	float v0 = std::min(maxSpeed, curveSpeed);

	// ---- Longitudinal acceleration (IDM or fuzzy hook) ----
	float accel = computeIDMAcceleration(
		currentSpeed, v0, maxAccel, maxDecel, perception
	);

	// ---- Block braking takes priority ----
	if (perception.hasBlockHazard && perception.hazardIsActive)
	{
		float blockAccel = computeBlockBraking(currentSpeed, maxDecel, perception);
		accel = std::min(accel, blockAccel);
	}

	// Forward tangent direction
	Vec2 tangent = travel.bezierDerivative(
		p[segment], p[segment + 1], p[segment + 2],
		std::min(t + lookahead, 1.0f)
	);
	Vec2 forward = tangent.normalized();

	out.acceleration = forward * accel;
	return out;
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

Vec2 BehaviorHuman::computeTargetPoint(
	Travel& travel,
	int     segment,
	float   t,
	float   lookahead
)
{
	const auto& p = travel.TravelPoints;
	float tLook = std::min(t + lookahead, 1.0f);

	return travel.bezier(
		p[segment],
		p[segment + 1],
		p[segment + 2],
		tLook
	);
}

float BehaviorHuman::computeIDMAcceleration(
	float                  v,
	float                  v0,
	float                  maxAccel,
	float                  maxDecel,
	const PerceptionState& perception
)
{
	// Takagi–Sugeno hook: if a derived class overrides
	// fuzzyLongitudinalOutput() it can replace the free-road term.
	// For now this calls the stub which simply returns the IDM value.

	constexpr float delta = 4.0f;   // acceleration exponent
	constexpr float s0 = 3.0f;   // minimum gap [m]
	constexpr float T = 1.2f;   // desired time headway [s]

	if (v0 < 0.001f) return std::clamp(-maxDecel, -maxDecel, maxAccel);

	float freeRoad = std::pow(v / v0, delta);
	float interaction = 0.0f;

	if (perception.hasCarAhead)
	{
		float s = std::max(perception.distanceToCarAhead, 0.1f);
		float dv = perception.relativeSpeed;

		float desiredGap =
			s0 + v * T +
			(v * dv) / (2.0f * std::sqrt(maxAccel * maxDecel + 0.001f));

		float ratio = desiredGap / s;
		interaction = ratio * ratio;
	}

	float accel = fuzzyLongitudinalOutput(v, v0, freeRoad, interaction);
	accel = maxAccel * (1.0f - accel);

	return std::clamp(accel, -maxDecel, maxAccel);
}

float BehaviorHuman::computeBlockBraking(
	float                  currentSpeed,
	float                  maxDecel,
	const PerceptionState& perception
)
{
	if (!perception.hasBlockHazard || !perception.hazardIsActive)
		return 0.0f;

	float distance = perception.hazardDistance;

	// Three-zone strategy (identical to old BehaviorModel::computeBlockAvoidanceDeceleration)
	if (distance < 5.0f)
	{
		// Emergency: full deceleration
		return -maxDecel;
	}
	else if (distance < 20.0f)
	{
		// Reaction zone: ramp 0 → full over 15 m
		float ratio = (distance - 5.0f) / 15.0f;   // 1.0 = far edge, 0.0 = near edge
		return -maxDecel * (1.0f - ratio);
	}
	else
	{
		// Safe zone: no additional braking
		return 0.0f;
	}
}

// ---------------------------------------------------------------------------
// Takagi–Sugeno fuzzy stub
//
// Signature: given normalised free-road term and interaction term (both
// produced by IDM), return the combined normalised output that will be
// multiplied by maxAccel.
//
// Default implementation: pure pass-through (IDM behaviour unchanged).
// Override in a subclass to swap in fuzzy inference without touching
// compute() or the IBehavior interface.
//
// Parameters:
//   v            current speed [m/s]
//   v0           desired speed [m/s]
//   freeRoadTerm (v/v0)^delta — already computed
//   interactTerm (s*/s)^2    — already computed; 0 if no car ahead
// ---------------------------------------------------------------------------

float BehaviorHuman::fuzzyLongitudinalOutput(
	float /*v*/,
	float /*v0*/,
	float freeRoadTerm,
	float interactTerm
)
{
	// TODO: replace with Takagi–Sugeno rule evaluation.
	// For now: return IDM combined term so caller does maxAccel*(1 - result).
	return freeRoadTerm + interactTerm;
}