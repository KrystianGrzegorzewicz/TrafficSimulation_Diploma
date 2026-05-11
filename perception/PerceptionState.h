#pragma once
#include "vehicles/CarState.h"

// Plain data — the result of one perception pass.
// Replaces the old Perception struct and removes BlockPerception as a
// separate concept: block hazard information lives here alongside car info.
struct PerceptionState
{
    // ---- Car ahead ----
    bool      hasCarAhead = false;
    CarState  carAhead = {};
    float     distanceToCarAhead = 0.0f;
    float     relativeSpeed = 0.0f;
    float     relativeAcceleration = 0.0f;

    // ---- Block hazard (formerly BlockPerception) ----
    bool      hasBlockHazard = false;
    int       hazardBlockIndex = -1;
    float     hazardDistance = 999999.0f;
    bool      hazardIsActive = false;
    float     hazardThreat = 0.0f;   // 0–1

    // ---- Dynamic FOV params (computed inside PerceptionHuman) ----
    float     fovDot = 0.3f;
    float     maxViewDistance = 80.0f;
};