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

    // =========================
    // 1. LOOKAHEAD + TARGET
    // =========================
    float lookahead = lookaheadBase + currentSpeed * lookaheadSpeedFactor;
    float tLook = std::min(t + lookahead, 1.0f);

    Vec2 p0 = p[segment];
    Vec2 p1 = p[segment + 1];
    Vec2 p2 = p[segment + 2];

    out.targetPoint = travel.bezier(p0, p1, p2, tLook);

    Vec2 tangent = travel.bezierDerivative(p0, p1, p2, tLook);
    Vec2 forward = tangent.normalized();

    // =========================
    // 2. SPEED LIMIT (curve)
    // =========================
    float curveSpeed = travel.computeSpeedLimitAhead(
        segment,
        t,
        lookahead,
        6.0f   // aLatMax
    );

    float v0 = std::min(maxSpeed, curveSpeed);
    float v = currentSpeed;

    // =========================
    // 3. IDM PARAMS
    // =========================
    float a = maxAccel;
    float b = maxDecel;

    float delta = 4.0f;
    float s0 = 3.0f;      // minimalny dystans
    float T = 1.2f;       // czas reakcji

    // =========================
    // 4. FREE ROAD
    // =========================
    float freeTerm = std::pow(v / v0, delta);

    // =========================
    // 5. INTERACTION
    // =========================
    float interaction = 0.0f;

    if (perception.hasCarAhead)
    {
        float s = std::max(perception.distanceToCarAhead, 0.1f);
        float dv = perception.relativeSpeed;

        float desiredGap =
            s0 + v * T + (v * dv) / (2.0f * std::sqrt(a * b + 0.001f));

        float ratio = desiredGap / s;

        interaction = ratio * ratio;
    }

    // =========================
    // 6. FINAL ACCEL (IDM)
    // =========================
    float accelScalar = a * (1.0f - freeTerm - interaction);

    accelScalar = std::clamp(accelScalar, -b, a);

    out.acceleration = forward * accelScalar;

    return out;
}