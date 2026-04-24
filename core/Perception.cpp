#include "core/Perception.h"
#include <limits>
#include <vector>

void updatePerception(Perception& p, const std::vector<CarState>& others)
{
    p.hasCarAhead = false;
    float bestDist = (std::numeric_limits<float>::max)();

    Vec2 forward = p.self.velocity.length() > 0.001f
        ? p.self.velocity.normalized()
        : Vec2(1, 0);

    for (const auto& o : others)
    {
        // nie porównuj z samym sobą
        if (&o == &p.self) continue;

        Vec2 to = o.position - p.self.position;

        float forwardDot = to.dot(forward);

        // szerokość "pasa percepcji"
        float lateral = fabs(to.dot(Vec2(-forward.y, forward.x)));

        float laneWidth = 2.5f; // dostrój

        if (forwardDot <= 0) continue;
        if (lateral > laneWidth) continue;

        float dist = to.length();

        if (dist < bestDist)
        {
            bestDist = dist;
            p.carAhead = o;
            p.hasCarAhead = true;
        }
        float relSpeed = (p.self.velocity - o.velocity).length();

        if (relSpeed > 0.1f)
        {
            float ttc = bestDist / relSpeed;

            if (ttc < 2.0f) // 2 sekundy
            {
                p.hasCarAhead = true;
                break;
            }
        }
    }


    if (p.hasCarAhead)
    {
        p.distanceToCarAhead = bestDist;

        p.relativeSpeed =
            p.self.velocity.length() - p.carAhead.velocity.length();

        p.relativeAcceleration =
            p.self.acceleration.length() - p.carAhead.acceleration.length();
    }
}