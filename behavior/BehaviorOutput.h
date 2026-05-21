#pragma once
#include "core/Vec2.h"

struct BehaviorOutput
{
	Vec2 acceleration;   // longitudinal (forward-tangent scaled)
	Vec2 targetPoint;    // lookahead point on path for SteeringModel
};