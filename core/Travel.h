#pragma once
#include "core/Vec2.h"
#include <vector>

class Travel {
private:
	int weight;
public:
	Travel(std::vector<Vec2> points, int w);

	std::vector<Vec2> TravelPoints;

	Vec2 bezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const;
	Vec2 bezierDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const;
	float bezierCurvature(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const;
	float bezierRadius(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const;
	float maxSpeedAt(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t, float aLatMax) const;
	float computeSpeedLimitAhead(int segment, float t, float lookaheadT, float aLatMax) const;
	int getWeight() const;
};