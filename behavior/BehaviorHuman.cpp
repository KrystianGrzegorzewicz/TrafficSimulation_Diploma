#include "behavior/BehaviorHuman.h"
#include <algorithm>
#include <cmath>

BehaviorHuman::BehaviorHuman(
	std::unique_ptr<ILongitudinalModel> model
)
	: longitudinalModel(std::move(model))
{
}

MotionCommand BehaviorHuman::compute(
	Travel& travel,
	int segment,
	float t,
	const CarState& self,
	float maxSpeed,
	float maxAccel,
	float maxDecel,
	float lookaheadBase,
	float lookaheadSpeedFactor,
	const PerceptionState& perception
)
{
	MotionCommand cmd;

	const auto& p = travel.TravelPoints;

	if (segment + 2 >= static_cast<int>(p.size()))
	{
		cmd.targetPoint = self.position;
		cmd.longitudinalAcceleration = -maxDecel;
		return cmd;
	}

	float speed = self.velocity.length();
	float lookaheadDist = std::max(6.0f, speed * 1.2f);
	float lookahead = lookaheadDist / 30.0f;

	// Target point (pure geometric planning)
	cmd.targetPoint = computeTargetPoint(travel, segment, t, lookahead);

	// Curvature-based speed limit.
	float aLatMax = 6.0f;
	float curveSpeed = travel.computeSpeedLimitAhead(
		segment, t, lookahead, aLatMax
	);
	/*	constexpr float kMinCurveSpeed = 8.0f;   // never crawl below ~28 km/h
		curveSpeed = std::max(curveSpeed, kMinCurveSpeed);*/
	float desiredSpeed = std::min(maxSpeed, curveSpeed);

	// Block hazard override
	if (perception.hasBlockHazard && perception.hazardIsActive)
	{
		if (perception.hazardDistance < 10.0f)
			desiredSpeed = 0.0f;
		else
			desiredSpeed *= 0.5f;
	}

	// Longitudinal model (IDM or fuzzy)
	float accel = longitudinalModel->computeAcceleration(
		self,
		perception,
		desiredSpeed,
		maxAccel,
		maxDecel
	);

	cmd.longitudinalAcceleration = accel;

	if (perception.hasBlockHazard && perception.hazardDistance < 5.0f)
		cmd.emergencyBrake = true;

	return cmd;
}

Vec2 BehaviorHuman::computeTargetPoint(
	Travel& travel,
	int segment,
	float t,
	float lookahead
)
{
	const auto& p = travel.TravelPoints;
	int   seg = segment;
	float localT = t;
	float remain = lookahead;

	// Walk forward through segments in t-space until we've consumed the
	// requested lookahead or run out of path. This stops the target point
	// from snapping onto the segment-end waypoint.
	while (remain > 0.f && seg + 2 < static_cast<int>(p.size()))
	{
		float room = 1.f - localT;
		if (remain <= room)
		{
			localT += remain;
			remain = 0.f;
			break;
		}
		remain -= room;
		seg += 2;
		localT = 0.f;
	}

	// If we ran off the end, aim at the very last point
	if (seg + 2 >= static_cast<int>(p.size()))
	{
		return p.back();
	}

	return travel.bezier(
		p[seg], p[seg + 1], p[seg + 2], localT
	);
}