#pragma once
#include "core/CarState.h"
#include "core/BlockPerception.h"
#include <vector>

struct FOVResult
{
    float fovDot;
    float maxViewDistance;
};

struct Perception
{
    CarState self;
    CarState carAhead;
    bool hasCarAhead = false;
    float distanceToCarAhead = 0.0f;
    float relativeSpeed = 0.0f;
    float relativeAcceleration = 0.0f;
    float minFovDot = 0.3f;  // szeroko przy małej prędkości (~70°)
    float maxFovDot = 0.9f;  // wąsko przy dużej (~25°)
    float maxSpeed = 30.0f;  // prędkość przy której osiągasz max zawężenie
    
    // Block hazard detection
    BlockHazard blockHazard;
    bool hasBlockHazard = false;
};

void updatePerception(Perception& p, const std::vector<CarState>& others);
FOVResult calculateFOV(const CarState& self, const Vec2& forward, const Vec2& right);