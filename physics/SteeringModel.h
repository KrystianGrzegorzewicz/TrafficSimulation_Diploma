#pragma once
#include "core/Vec2.h"
#include "behavior/BehaviorOutput.h"
#include "road/Travel.h"

// Pure lateral-acceleration controller.
// Knows nothing about Behavior or Perception strategies —
// receives only the BehaviorOutput target point and current kinematic state.
class SteeringModel
{
public:
    SteeringModel(float kp, float kd) : kp(kp), kd(kd) {}

    Vec2 computeLateralAcceleration(
        const Travel& travel,
        int                   segment,
        float                 t,
        const Vec2& position,
        const Vec2& velocity,
        float                 lookaheadBase,
        float                 lookaheadSpeedFactor,
        const BehaviorOutput& behaviorOut
    ) const;

private:
    float kp;
    float kd;
    float aLatMax = 6.0f;
    float minTurnRadius = 2.0f;
};