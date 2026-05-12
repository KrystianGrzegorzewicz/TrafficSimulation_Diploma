#pragma once

#include "behavior/MotionCommand.h"
#include "perception/PerceptionState.h"
#include "road/Travel.h"
#include "vehicles/CarState.h"

// High-level vehicle behavior.
//
// Responsible for motion planning:
// - desired speed
// - longitudinal acceleration
// - lookahead target point
class IBehavior
{
public:
    virtual ~IBehavior() = default;

    virtual MotionCommand compute(
        Travel& travel,
        int segment,
        float t,
        const CarState& self,
        float maxSpeed,
        float maxAccel,
        float maxDecel,
        float lookaheadBase,
        float lookaheadSpeedFactor,
        const PerceptionState& perception
    ) = 0;
};