#include "planning/PathPlanner.h"

#include <algorithm>

struct ArcLengthTarget
{
	Vec2 point;
	int segment;
	float t;
};

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

	float traveled = 0.0f;

	const float step = 0.01f;

	while (seg + 2 < (int)p.size())
	{
		Vec2 prev =
			travel.bezier(
				p[seg],
				p[seg + 1],
				p[seg + 2],
				localT);

		float nextT = localT + step;

		if (nextT > 1.0f)
		{
			seg += 2;

			if (seg + 2 >= (int)p.size())
				return p.back();

			localT = 0.0f;
			continue;
		}

		Vec2 next =
			travel.bezier(
				p[seg],
				p[seg + 1],
				p[seg + 2],
				nextT);

		float ds = (next - prev).length();

		if (traveled + ds >= lookaheadDist)
		{
			float alpha =
				(lookaheadDist - traveled) /
				std::max(ds, 0.0001f);

			return prev + (next - prev) * alpha;
		}

		traveled += ds;
		localT = nextT;
	}

	return p.back();
}