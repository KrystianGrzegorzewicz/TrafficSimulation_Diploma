#pragma once
#include "core/Vec2.h"
#include <vector>

class Travel {
private:
	
public:
	Travel(std::vector<Vec2> points);
	Vec2 bezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
	Vec2 bezierDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
	float bezierCurvature(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
	float bezierRadius(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);

	// nowa rzecz: ograniczenie prędkości w danym t
	float maxSpeedAt(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t, float aLatMax);

	// preview – najważniejsze
	float computeSpeedLimitAhead(int segment, float t, float lookaheadT, float aLatMax);

	std::vector<Vec2> TravelPoints;
};