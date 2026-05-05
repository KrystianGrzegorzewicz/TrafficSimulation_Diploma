#include "core/BehaviorModel.h"
#include <algorithm>
#include <cmath>

BehaviorOutput BehaviorModel::compute(
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
)
{
    BehaviorOutput out;
    auto& p = travel.TravelPoints;

    if (segment + 2 >= p.size())
    {
        out.acceleration = Vec2(0, 0);
        out.targetPoint = p.back();
        return out;
    }

    float lookahead =
        lookaheadBase + currentSpeed * lookaheadSpeedFactor;

    out.targetPoint =
        computeTargetPoint(travel, segment, t, lookahead);

    Vec2 tangent =
        travel.bezierDerivative(
            p[segment], p[segment + 1], p[segment + 2],
            std::min(t + lookahead, 1.0f)
        );

    Vec2 forward = tangent.normalized();

    float curveSpeed =
        travel.computeSpeedLimitAhead(segment, t, lookahead, 6.0f);

    float v0 = std::min(maxSpeed, curveSpeed);

    float accel =
        computeIDMAcceleration(
            currentSpeed,
            v0,
            maxAccel,
            maxDecel,
            perception
        );

    out.acceleration = forward * accel;
    return out;
}

Vec2 BehaviorModel::computeTargetPoint(
    Travel& travel,
    int segment,
    float t,
    float lookahead
)
{
    auto& p = travel.TravelPoints;
    float tLook = std::min(t + lookahead, 1.0f);

    return travel.bezier(
        p[segment],
        p[segment + 1],
        p[segment + 2],
        tLook
    );
}

float BehaviorModel::computeIDMAcceleration(
    float v,
    float v0,
    float maxAccel,
    float maxDecel,
    const Perception& perception
)
{
    constexpr float delta = 4.0f;
    constexpr float s0 = 3.0f;
    constexpr float T = 1.2f;

    float freeRoad = std::pow(v / v0, delta);
    float interaction = 0.0f;

    if (perception.hasCarAhead)
    {
        float s = std::max(perception.distanceToCarAhead, 0.1f);
        float dv = perception.relativeSpeed;

        float desiredGap =
            s0 + v * T +
            (v * dv) / (2.0f * std::sqrt(maxAccel * maxDecel + 0.001f));

        interaction = (desiredGap / s) * (desiredGap / s);
    }

    float accel =
        maxAccel * (1.0f - freeRoad - interaction);

    return std::clamp(accel, -maxDecel, maxAccel);
}