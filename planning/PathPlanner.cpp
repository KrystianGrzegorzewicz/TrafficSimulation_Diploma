// planning/PathPlanner.cpp
#include "planning/PathPlanner.h"
#include <algorithm>

PathPlan PathPlanner::compute(
	Travel& travel,
	int segment,
	float t,
	float speed,
	float aLatMax)
{
	PathPlan out;

	float lookaheadDist = std::max(6.0f, speed * 1.2f);
	out.lookaheadDistance = lookaheadDist;

	// Convert meters -> parametric t step using derivative magnitude
	const auto& p = travel.TravelPoints;
	Vec2 d = travel.bezierDerivative(p[segment], p[segment + 1], p[segment + 2], t);
	float metersPerT = std::max(d.length(), 0.001f);
	float lookaheadT = lookaheadDist / metersPerT;

	out.targetPoint = computeTargetArcLength(travel, segment, t, lookaheadDist);

	out.maxCurveSpeed = travel.computeSpeedLimitAhead(
		segment, t, lookaheadT, aLatMax);

	return out;
}

Vec2 PathPlanner::computeTargetArcLength(
	Travel& travel,
	int segment,
	float t,
	float lookaheadDist)
{
	const auto& p = travel.TravelPoints;

	int seg = segment;
	float localT = t;
	float remainMeters = lookaheadDist;

	while (remainMeters > 0 && seg + 2 < (int)p.size())
	{
		Vec2 deriv = travel.bezierDerivative(p[seg], p[seg + 1], p[seg + 2], localT);
		float metersPerT = std::max(deriv.length(), 0.001f);
		float dt = remainMeters / metersPerT;

		if (localT + dt <= 1.0f)
		{
			localT += dt;
			remainMeters = 0;
			break;
		}

		remainMeters -= (1.0f - localT) * metersPerT;
		seg += 2;
		localT = 0;
	}

	if (seg + 2 >= (int)p.size()) return p.back();
	return travel.bezier(p[seg], p[seg + 1], p[seg + 2], localT);
}