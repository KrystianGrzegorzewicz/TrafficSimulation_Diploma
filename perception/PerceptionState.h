#pragma once

#include "vehicles/CarState.h"

struct PerceptionState
{
	bool hasCarAhead = false;
	CarState carAhead = {};
	float distanceToCarAhead = 0.0f;
	float relativeSpeed = 0.0f;
	float relativeAcceleration = 0.0f;

	bool hasBlockHazard = false;
	int hazardBlockIndex = -1;
	float hazardDistance = 999999.0f;
	bool hazardIsActive = false;
	float hazardThreat = 0.0f;

	float fovDot = 0.3f;
	float maxViewDistance = 80.0f;
};