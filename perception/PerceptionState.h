#pragma once

#include "core/Vec2.h"
#include "vehicles/CarState.h"
#include <vector>

struct PriorityCarTTA
{
	int carId = -1;
	float ttaEntry = 999999.f; // Czas do wjazdu w strefę (s)
	float ttaExit = 999999.f;  // Czas do opuszczenia strefy (s)
	float distanceToCp = 999999.f;
	bool isAV = false;
};

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

	bool hasConflict = false;
	bool alreadyEnteringConflict = false;
	CarState conflictingCar;
	float conflictDistance = 999999.f;
	float myArrival = 999999.f;
	float otherArrival = 999999.f;
	float conflictThreat = 0.f;
	float selfTtaEntry = 999999.f;
	float selfTtaExit = 999999.f;
	std::vector<PriorityCarTTA> priorityCarsTTA;
};