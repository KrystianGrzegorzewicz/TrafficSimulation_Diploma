#include "core/Perception.h"
#include <limits>
#include <vector>

void updatePerception(Perception& p, const std::vector<CarState>& others)
{
    p.hasCarAhead = false;
    p.distanceToCarAhead = 0.0f;
    p.relativeSpeed = 0.0f;
    p.relativeAcceleration = 0.0f;

    float bestTTC = std::numeric_limits<float>::max();
    float bestDist = std::numeric_limits<float>::max();

    Vec2 forward = (p.self.velocity.length() > 0.001f)
        ? p.self.velocity.normalized()
        : Vec2(1, 0);

    Vec2 right(-forward.y, forward.x);

    for (const auto& o : others)
    {
        // pomijamy "siebie" po pozycji (bez pointerów!)
        if ((o.position - p.self.position).length() < 0.0001f)
            continue;

        Vec2 relPos = o.position - p.self.position;
        Vec2 relVel = o.velocity - p.self.velocity;

        float forwardDist = relPos.dot(forward);
        float lateralDist = std::fabs(relPos.dot(right));

        float laneWidth = 2.5f;

        // auto musi być "przed nami w pasie"
        if (forwardDist <= 0.0f) continue;
        if (lateralDist > laneWidth) continue;

        // =========================
        // TIME TO COLLISION (TTC)
        // =========================
        float closingSpeed = -relPos.dot(relVel);

        if (closingSpeed < 0.01f)
            continue;

        if (closingSpeed > 0.01f)
        {
            float ttc = relPos.length() / closingSpeed;

            float safeTTC = 2.0f;

            if (ttc < safeTTC && ttc < bestTTC)
            {
                bestTTC = ttc;
                bestDist = relPos.length();

                p.carAhead = o;
                p.hasCarAhead = true;
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