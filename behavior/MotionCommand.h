#pragma once

#include "core/Vec2.h"

struct MotionCommand {
	Vec2 targetPoint;
	Vec2 targetTangent;
	float longitudinalAcceleration = 0;
	float internalDt = 0.01f;
	bool emergencyBrake = false;
	float desiredSpeed = 0.0f;
};