#pragma once
#include "core/Vec2.h"
#include "core/BehaviorModel.h"
#include "core/Travel.h"

class SteeringModel
{
public:
    SteeringModel(float kp, float kd)
        : kp(kp), kd(kd) {
    }

    Vec2 computeLateralAcceleration(
        const Travel& travel,
        int segment,
        float t,
        const Vec2& position,
        const Vec2& velocity,
        float lookaheadBase,
        float lookaheadSpeedFactor,
        const BehaviorOutput& behaviorOut
    ) const;

private:
    float kp;
    float kd;

    // maksymalne dopuszczalne przyspieszenie boczne (fizyka / komfort)
    float aLatMax = 6.0f;

    // minimalny promień skrętu (metry)
    float minTurnRadius = 2.0f;
};