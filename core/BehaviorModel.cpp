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
    float lookaheadSpeedFactor
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

    // =========================
    // dynamic lookahead (TAK JAK MIAŁEŚ)
    // =========================
    float dynamicLookahead = lookaheadBase + currentSpeed * lookaheadSpeedFactor;

    float tLook = std::min(t + dynamicLookahead, 1.0f);

    // =========================
    // target point (TO SAMO CO BYŁO)
    // =========================
    out.targetPoint = travel.bezier(
        p[segment],
        p[segment + 1],
        p[segment + 2],
        tLook
    );

    // =========================
    // speed limit (TAK SAMO)
    // =========================
    float curveSpeed = travel.computeSpeedLimitAhead(
        segment,
        t,
        dynamicLookahead,
        aLatMax
    );

    out.targetSpeed = std::min(maxSpeed, curveSpeed);

    return out;
}