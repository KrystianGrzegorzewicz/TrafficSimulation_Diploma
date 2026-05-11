#include "road/Travel.h"
#include <cmath>
#include <limits>

Travel::Travel(std::vector<Vec2> points, int w, int id)
	: TravelPoints(std::move(points)), weight(w), id(id)
{
}

// ---------------------------------------------------------------------------
// Quadratic Bézier evaluation
// ---------------------------------------------------------------------------

Vec2 Travel::bezier(
	const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const
{
	float u = 1.f - t;
	return p0 * (u * u) + p1 * (2.f * u * t) + p2 * (t * t);
}

Vec2 Travel::bezierDerivative(
	const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const
{
	return (p1 - p0) * (2.f * (1.f - t)) + (p2 - p1) * (2.f * t);
}

// ---------------------------------------------------------------------------
// Curvature κ = |B'×B''| / |B'|³
// ---------------------------------------------------------------------------

float Travel::bezierCurvature(
	const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const
{
	Vec2  d1 = bezierDerivative(p0, p1, p2, t);
	Vec2  d2 = (p2 - p1 * 2.f + p0) * 2.f;

	float num = std::fabs(d1.x * d2.y - d1.y * d2.x);
	float denom = std::pow(d1.x * d1.x + d1.y * d1.y, 1.5f);

	if (denom < 0.0001f) return 0.f;
	return num / denom;
}

float Travel::bezierRadius(
	const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const
{
	float k = bezierCurvature(p0, p1, p2, t);
	if (k < 0.00001f) return 999999.f;
	return 1.f / k;
}

// ---------------------------------------------------------------------------
// Maximum cornering speed at a given t: v = sqrt(aLatMax * R)
// ---------------------------------------------------------------------------

float Travel::maxSpeedAt(
	const Vec2& p0, const Vec2& p1, const Vec2& p2,
	float t, float aLatMax) const
{
	float R = bezierRadius(p0, p1, p2, t);
	return std::sqrt(aLatMax * R);
}

// ---------------------------------------------------------------------------
// Minimum speed limit over a lookahead window
// ---------------------------------------------------------------------------

float Travel::computeSpeedLimitAhead(
	int segment, float t, float lookaheadT, float aLatMax) const
{
	float minSpeed = std::numeric_limits<float>::max();
	int   seg = segment;
	float localT = t;

	const float STEP = 0.02f;
	float       traveled = 0.f;

	while (traveled < lookaheadT &&
		seg + 2 < static_cast<int>(TravelPoints.size()))
	{
		const Vec2& p0 = TravelPoints[seg];
		const Vec2& p1 = TravelPoints[seg + 1];
		const Vec2& p2 = TravelPoints[seg + 2];

		float v = maxSpeedAt(p0, p1, p2, localT, aLatMax);
		minSpeed = std::min(minSpeed, v);

		localT += STEP;
		traveled += STEP;

		if (localT >= 1.f)
		{
			seg += 2;
			localT = 0.f;
		}
	}

	if (minSpeed == std::numeric_limits<float>::max())
		return 50.f;   // no curvature constraint found — open road

	return minSpeed;
}

int Travel::getWeight() const { return weight; }
int Travel::getId()     const { return id; }