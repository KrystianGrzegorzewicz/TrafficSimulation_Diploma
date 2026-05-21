#pragma once
#include "road/Travel.h"
#include "core/Vec2.h"

struct PathPlan {
	Vec2 targetPoint;
	float maxCurveSpeed;
	float lookaheadDistance;
};

class PathPlanner {
public:
	PathPlan compute(Travel& travel, int segment, float t, float speed, float aLatMax);

private:
	Vec2 computeTargetArcLength(Travel& travel, int segment, float t, float lookaheadDist);
};