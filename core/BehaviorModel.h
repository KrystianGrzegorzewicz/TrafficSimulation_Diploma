#pragma once
#include "core/Travel.h"
#include "core/Perception.h"

struct BehaviorOutput
{
    float targetSpeed;
    Vec2 targetPoint;
};

class BehaviorModel
{
public:
    BehaviorOutput compute(
        Travel& travel,
        int segment,
        float t,
        float currentSpeed,
        float maxSpeed,
        float aLatMax,
        float lookaheadBase,
        float lookaheadSpeedFactor,
        const Perception& perception
    );
};

float computeSafeDistance(float speed, float maxDecel);