#pragma once

#include "behavior/IBehavior.h"
#include "behavior/longitudinal/ILongitudinalModel.h"
#include <memory>

class BehaviorHuman : public IBehavior
{
public:
    explicit BehaviorHuman(
        std::unique_ptr<ILongitudinalModel> longitudinalModel
    );

    MotionCommand compute(
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
    ) override;

private:
    Vec2 computeTargetPoint(
        Travel& travel,
        int segment,
        float t,
        float lookahead
    );

    float computeBlockBraking(
        float currentSpeed,
        float maxDecel,
        const PerceptionState& perception
    );

private:
    std::unique_ptr<ILongitudinalModel>
        longitudinalModel;
};