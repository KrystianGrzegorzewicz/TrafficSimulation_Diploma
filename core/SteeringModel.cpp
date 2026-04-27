#include "core/SteeringModel.h"
#include <algorithm>

Vec2 SteeringModel::computeLateralAcceleration(
    const Travel& travel,
    int segment,
    float t,
    const Vec2& position,
    const Vec2& velocity,
    float lookaheadBase,
    float lookaheadSpeedFactor,
    const BehaviorOutput& behaviorOut
) const
{
    auto& p = travel.TravelPoints;

    Vec2 p0 = p[segment];
    Vec2 p1 = p[segment + 1];
    Vec2 p2 = p[segment + 2];

    float lookahead =
        lookaheadBase + velocity.length() * lookaheadSpeedFactor;

    float tLook = std::min(t + lookahead, 1.0f);

    Vec2 tangent =
        travel.bezierDerivative(p0, p1, p2, tLook);

    Vec2 forward = tangent.normalized();
    Vec2 right(-forward.y, forward.x);

    Vec2 toTarget = behaviorOut.targetPoint - position;

    float forwardMag = toTarget.dot(forward);
    Vec2 lateralError = toTarget - forward * forwardMag;

    float lateralVel = velocity.dot(right);

    return lateralError * kp - right * lateralVel * kd;
}