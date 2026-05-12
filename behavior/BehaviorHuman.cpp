#include "behavior/BehaviorHuman.h"
#include <algorithm>
#include <cmath>

BehaviorHuman::BehaviorHuman(
    std::unique_ptr<ILongitudinalModel> model
)
    : longitudinalModel(std::move(model))
{
}

MotionCommand BehaviorHuman::compute(
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
)
{
    MotionCommand cmd;

    const auto& p = travel.TravelPoints;

    if (segment + 2 >= static_cast<int>(p.size()))
    {
        cmd.targetPoint = self.position;
        cmd.longitudinalAcceleration = -maxDecel;
        return cmd;
    }

    float speed = self.velocity.length();
    float lookahead = lookaheadBase + speed * lookaheadSpeedFactor;

    // Target point (pure geometric planning)
    cmd.targetPoint = computeTargetPoint(travel, segment, t, lookahead);

    // Curvature-based speed limit
    float curveSpeed = travel.computeSpeedLimitAhead(
        segment, t, lookahead, 6.0f
    );

    float desiredSpeed = std::min(maxSpeed, curveSpeed);

    // Block hazard override
    if (perception.hasBlockHazard && perception.hazardIsActive)
    {
        if (perception.hazardDistance < 10.0f)
            desiredSpeed = 0.0f;
        else
            desiredSpeed *= 0.5f;
    }

    // Longitudinal model (IDM or fuzzy)
    float accel = longitudinalModel->computeAcceleration(
        self,
        perception,
        desiredSpeed,
        maxAccel,
        maxDecel
    );

    cmd.longitudinalAcceleration = accel;

    if (perception.hasBlockHazard && perception.hazardDistance < 5.0f)
        cmd.emergencyBrake = true;

    return cmd;
}

Vec2 BehaviorHuman::computeTargetPoint(
    Travel& travel,
    int segment,
    float t,
    float lookahead
)
{
    const auto& p = travel.TravelPoints;

    float tLook = std::min(t + lookahead, 1.0f);

    return travel.bezier(
        p[segment],
        p[segment + 1],
        p[segment + 2],
        tLook
    );
}