#include "core/BehaviorModel.h"
#include <algorithm>

BehaviorOutput BehaviorModel::compute(
    Travel& travel,
    int segment,
    float t,
    float currentSpeed,
    float maxSpeed,
    float aLatMax,
    float lookaheadBase,
    float lookaheadSpeedFactor,
    const Perception& perception
)
{
    BehaviorOutput out;

    if (segment + 2 >= travel.TravelPoints.size())
    {
        out.targetSpeed = 0;
        out.targetPoint = travel.TravelPoints.back();
        return out;
    }

    auto& p = travel.TravelPoints;
    float dynamicLookahead = lookaheadBase + currentSpeed * lookaheadSpeedFactor;

    float tLook = std::min(t + dynamicLookahead, 1.0f);

    out.targetPoint = travel.bezier(
        p[segment], p[segment + 1], p[segment + 2], tLook);
    float curveSpeed = travel.computeSpeedLimitAhead(
        segment,
        t,
        dynamicLookahead,
        aLatMax
    );

    // =========================
    // OGRANICZENIE PRZEZ AUTO Z PRZODU
    // =========================
    // =========================
// REAKCJA NA AUTO Z PRZODU (rozszerzona)
// =========================
    if (perception.hasCarAhead)
    {
        float safeDist = 2.0f + currentSpeed * 1.5f;

        // 1. klasyczne dopasowanie dystansu
        if (perception.distanceToCarAhead < safeDist)
        {
            out.targetSpeed = std::min(
                out.targetSpeed,
                perception.carAhead.velocity.length()
            );
        }

        // 2. 🔥 NOWE: reakcja na hamowanie auta z przodu
        float frontAcc = perception.carAhead.acceleration.length();

        if (frontAcc < -1.0f) // auto przed nami hamuje
        {
            // reaguj wcześniej
            float brakeFactor = std::clamp(
                (-frontAcc) / 5.0f,
                0.0f,
                1.0f
            );

            out.targetSpeed *= (1.0f - 0.7f * brakeFactor);
        }
    }

    out.targetSpeed = std::min(maxSpeed, curveSpeed);

    return out;
}