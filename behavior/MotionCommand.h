#pragma once
#include "core/Vec2.h"

struct MotionCommand {
	Vec2 targetPoint; // from PathPlanning
	float longitudinalAcceleration = 0; // from LongitudinalModel
	bool emergencyBrake = false;
	Vec2 targetTangent;
	float internalDt = 0.01f;
};