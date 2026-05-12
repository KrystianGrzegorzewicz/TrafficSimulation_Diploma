#pragma once
#include "core/Vec2.h"

// Motion-level command produced by Behavior.
//
// Separates planning from control:
// - targetPoint -> lateral controller goal
// - longitudinalAcceleration -> desired forward acceleration
//
// This replaces the old BehaviorOutput which mixed
// planning and control concepts.
struct MotionCommand
{
    Vec2 targetPoint;

    // Desired acceleration along vehicle forward direction [m/s^2]
    float longitudinalAcceleration = 0.0f;

    bool emergencyBrake = false;
};