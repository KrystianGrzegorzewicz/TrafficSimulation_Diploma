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
	const auto& pts = travel.TravelPoints;

	float curvature =
		travel.bezierCurvature(
			pts[segment],
			pts[segment + 1],
			pts[segment + 2],
			t);

	float radius =
		travel.bezierRadius(
			pts[segment],
			pts[segment + 1],
			pts[segment + 2],
			t);

	float curveFactor = std::clamp(radius / 25.f, 0.2f, 1.0f);

	float lookaheadDist = (2.0f + speed * 0.4f) * curveFactor;

	out.lookaheadDistance = lookaheadDist;
	out.targetPoint =
		computeTargetArcLength(
			travel,
			segment,
			t,
			lookaheadDist);

	Vec2 tangent =
		travel.bezierDerivative(
			pts[segment],
			pts[segment + 1],
			pts[segment + 2],
			std::min(t + 0.1f, 1.0f));

	out.targetTangent = tangent.normalized();
	float reactionTime = 3.0f;
	float brakingDistance = (speed * speed) / (2.f * 5.0f);

	float previewDistance = std::max(25.f, speed * reactionTime + brakingDistance);
	out.maxCurveSpeed = travel.computeSpeedLimitAhead(segment, t, previewDistance, aLatMax);

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