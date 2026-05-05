#include "core/PerceptionUpdater.h"
#include <limits>
#include <cmath>

void PerceptionUpdater::update(
    Perception& p,
    const std::vector<CarState>& others
)
{
    p.hasCarAhead = false;
    float bestScore = std::numeric_limits<float>::max();

    Vec2 forward =
        (p.self.velocity.length() > 0.5f)
        ? p.self.velocity.normalized()
        : Vec2(1, 0);

    Vec2 right(-forward.y, forward.x);

    constexpr float maxViewDistance = 50.0f;
    constexpr float laneWidth = 3.0f;
    constexpr float fovDot = 0.3f;

    for (const auto& o : others)
    {
        Vec2 relPos = o.position - p.self.position;
        float dist = relPos.length();

        if (dist < 0.001f || dist > maxViewDistance)
            continue;

        Vec2 dir = relPos / dist;

        if (forward.dot(dir) < fovDot)
            continue;

        if (std::fabs(relPos.dot(right)) > laneWidth)
            continue;

        Vec2 relVel = o.velocity - p.self.velocity;
        float closingSpeed = -relVel.dot(dir);

        if (closingSpeed <= 0.1f)
            continue;

        float ttc = dist / closingSpeed;

        if (ttc < bestScore)
        {
            bestScore = ttc;

            p.hasCarAhead = true;
            p.carAhead = o;
            p.distanceToCarAhead = dist;
            p.relativeSpeed =
                p.self.velocity.length() - o.velocity.length();
            p.relativeAcceleration =
                p.self.acceleration.length() - o.acceleration.length();
        }
    }
}