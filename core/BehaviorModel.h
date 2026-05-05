#pragma once
#include "core/Vec2.h"
#include "core/Perception.h"
#include "core/Travel.h"

struct BehaviorOutput
{
    Vec2 acceleration;
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
        float maxAccel,
        float maxDecel,
        float lookaheadBase,
        float lookaheadSpeedFactor,
        const Perception& perception
    );

private:
    Vec2 computeTargetPoint(
        Travel& travel,
        int segment,
        float t,
        float lookahead
    );

    float computeIDMAcceleration(
        float v,
        float v0,
        float maxAccel,
        float maxDecel,
        const Perception& perception
    );
    
    float computeBlockAvoidanceDeceleration(
        float currentSpeed,
        float maxDecel,
        const Perception& perception
    );
};