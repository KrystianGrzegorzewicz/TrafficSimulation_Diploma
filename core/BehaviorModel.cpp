#include "core/BehaviorModel.h"
#include <algorithm>

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

    // =========================
    // GEOMETRIA (BÉZIER FOLLOW)
    // =========================
    float lookahead = lookaheadBase + currentSpeed * lookaheadSpeedFactor;
    float tLook = std::min(t + lookahead, 1.0f);

    out.targetPoint = travel.bezier(
        p[segment], p[segment + 1], p[segment + 2], tLook
    );

    Vec2 tangent = travel.bezierDerivative(
        p[segment], p[segment + 1], p[segment + 2], tLook
    );

    Vec2 forward = tangent.normalized();

    // =========================
    // IDM CORE
    // =========================

    float v = currentSpeed;
    float v0 = maxSpeed;

    float freeAccel = maxAccel * (1.0f - (v / v0));

    float interactionAccel = 0.0f;

    if (perception.hasCarAhead)
    {
        float s = perception.distanceToCarAhead;
        float dv = perception.relativeSpeed;

        float s0 = 2.0f;
        float T = 1.2f;

        float desiredGap =
            s0 + v * T + (v * dv) / (2.0f * std::sqrt(maxAccel * maxDecel + 0.001f));

        float ratio = desiredGap / std::max(s, 0.1f);

        interactionAccel =
            -maxAccel * ratio * ratio;
    }

    float a = freeAccel + interactionAccel;

    // clamp fizyczny
    a = std::clamp(a, -maxDecel, maxAccel);

    out.acceleration = forward * a;

    return out;
}

float computeSafeDistance(float speed, float maxDecel)
{
    float reactionTime = 0.5f;

    float reactionDist = speed * reactionTime;
    float brakingDist = (speed * speed) / (2.0f * maxDecel);

    return reactionDist + brakingDist;
}