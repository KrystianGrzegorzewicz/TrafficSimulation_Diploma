#pragma once

#include "vehicles/CarState.h"
#include "perception/PerceptionState.h"

// Interface for longitudinal (forward) motion models.
//
// Responsible ONLY for acceleration/braking decisions.
//
// Examples:
// - IDM (human-like following)
// - Fuzzy logic controller
// - MPC / RL controller for AV
class ILongitudinalModel
{
public:
    virtual ~ILongitudinalModel() = default;

    virtual float computeAcceleration(
        const CarState& self,
        const PerceptionState& perception,
        float desiredSpeed,
        float maxAccel,
        float maxDecel
    ) = 0;
};