#pragma once

#include "core/Vec2.h"

// Pure lateral controller.
//
// Literature:
// Classical PD trajectory tracking controller.
//
// Steering computes ONLY lateral acceleration.
// No planning.
// No perception.
// No road geometry.
class SteeringModel
{
public:
    SteeringModel(float kp, float kd)
        : kp(kp), kd(kd) {
    }

    Vec2 computeLateralAcceleration(
        const Vec2& position,
        const Vec2& velocity,
        const Vec2& targetPoint
    ) const;

private:
    float kp;
    float kd;

    float aLatMax = 6.0f;
    float minTurnRadius = 2.0f;
};