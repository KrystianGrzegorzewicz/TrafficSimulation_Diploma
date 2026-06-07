#include "road/Travel.h"

#include <cmath>
#include <limits>

Travel::Travel(std::vector<Vec2> points, int w, int id)
	: TravelPoints(std::move(points)), weight(w), id(id)
{
}

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

float Travel::maxSpeedAt(
	const Vec2& p0, const Vec2& p1, const Vec2& p2,
	float t, float aLatMax) const
{
	float R = bezierRadius(p0, p1, p2, t);
	return std::sqrt(aLatMax * R);
}

float Travel::computeSpeedLimitAhead(
	int segment,
	float t,
	float lookaheadDistance,
	float aLatMax) const
{
	struct SpeedSample
	{
		float distance;
		float curveSpeed;
	};

	std::vector<SpeedSample> samples;

	int seg = segment;
	float localT = t;
	float traveled = 0.f;
	const float step = 0.01f;

	while (traveled < lookaheadDistance && seg + 2 < (int)TravelPoints.size())
	{
		const Vec2& p0 = TravelPoints[seg];
		const Vec2& p1 = TravelPoints[seg + 1];
		const Vec2& p2 = TravelPoints[seg + 2];

		float curvature = bezierCurvature(p0, p1, p2, localT);
		float curveSpeed;

		if (curvature < 1e-5f)
		{
			curveSpeed = 999.f;
		}
		else
		{
			float radius = 1.f / curvature;
			curveSpeed = std::sqrt(aLatMax * radius) * 0.72f;
		}

		samples.push_back({ traveled,curveSpeed });

		Vec2 deriv = bezierDerivative(p0, p1, p2, localT);
		float ds = deriv.length() * step;

		traveled += ds;
		localT += step;

		if (localT >= 1.f)
		{
			seg += 2;
			localT = 0.f;
		}
	}

	if (samples.empty())
		return 999.f;

	const float planningDecel = 4.5f;
	float allowedSpeed = samples.back().curveSpeed;

	for (int i = (int)samples.size() - 2; i >= 0; --i)
	{
		float ds = samples[i + 1].distance - samples[i].distance;
		float maxEnterSpeed = std::sqrt(allowedSpeed * allowedSpeed + 2.f * planningDecel * ds);

		allowedSpeed = std::min(samples[i].curveSpeed, maxEnterSpeed);
	}

	return allowedSpeed;
}

int Travel::getWeight() const { return weight; }
int Travel::getId() const { return id; }