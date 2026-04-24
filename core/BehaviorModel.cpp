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
        float safeDist = computeSafeDistance(
            currentSpeed,
            aLatMax   // albo lepiej maxDecel jeśli masz
        );
        safeDist *= 1.2f; // safety buffer

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

    float finalSpeed = std::min(maxSpeed, curveSpeed);

    // uwzględnij przeszkody NA KOŃCU (nie nadpisuj)
    if (perception.hasCarAhead)
    {
        float safeDist = computeSafeDistance(
            currentSpeed,
            aLatMax   // albo lepiej maxDecel jeśli masz
        );
        safeDist *= 1.2f; // safety buffer

        if (perception.distanceToCarAhead < safeDist)
        {
            finalSpeed = std::min(finalSpeed,
                perception.carAhead.velocity.length());
        }

        float frontAcc = perception.carAhead.acceleration.length();

        if (frontAcc < 0.0f)
        {
            finalSpeed *= 0.7f;
        }
    }

    out.targetSpeed = finalSpeed;

    return out;
}

float computeSafeDistance(float speed, float maxDecel)
{
    float reactionTime = 0.5f;

    float reactionDist = speed * reactionTime;
    float brakingDist = (speed * speed) / (2.0f * maxDecel);

    return reactionDist + brakingDist;
}