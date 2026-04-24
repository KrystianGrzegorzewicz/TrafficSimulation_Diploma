#pragma once
#include "core/Travel.h"
#include "core/Perception.h"

struct BehaviorOutput
{
    Vec2 acceleration;   // IDM output
    Vec2 targetPoint;    // dalej używane do ścieżki
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
        float maxAccel,
        float maxDecel,
        float lookaheadBase,
        float lookaheadSpeedFactor,
        const Perception& perception
    );
};

float computeSafeDistance(float speed, float maxDecel);