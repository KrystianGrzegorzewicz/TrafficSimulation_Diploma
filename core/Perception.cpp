#include "core/Perception.h"
#include <limits>
#include <cmath>

void updatePerception(Perception& p, const std::vector<CarState>& others)
{
    p.hasCarAhead = false;
    p.distanceToCarAhead = 0.0f;
    p.relativeSpeed = 0.0f;
    p.relativeAcceleration = 0.0f;

    float bestScore = std::numeric_limits<float>::max();

    // =========================
    // kierunek jazdy
    // =========================
    Vec2 forward = (p.self.velocity.length() > 0.5f)
        ? p.self.velocity.normalized()
        : Vec2(1, 0); // fallback (można później poprawić na kierunek z krzywej)

    Vec2 right(-forward.y, forward.x);

    // =========================
    // parametry percepcji
    // =========================
    float maxViewDistance = 50.0f;   // ile metrów widzi
    float laneWidth = 3.0f;          // szerokość pasa
    float fovDot = 0.3f;             // kąt widzenia (~70°)

    for (const auto& o : others)
    {
        // pomiń siebie
        if ((o.position - p.self.position).length() < 0.001f)
            continue;

        Vec2 relPos = o.position - p.self.position;
        float dist = relPos.length();

        if (dist > maxViewDistance)
            continue;

        Vec2 dir = relPos / dist;

        // =========================
        // filtr kąta (FOV)
        // =========================
        float dot = forward.dot(dir);
        if (dot < fovDot)
            continue;

        // =========================
        // filtr pasa
        // =========================
        float lateral = std::fabs(relPos.dot(right));
        if (lateral > laneWidth)
            continue;

        // =========================
        // względna prędkość
        // =========================
        Vec2 relVel = o.velocity - p.self.velocity;

        float closingSpeed = -relVel.dot(dir);

        // jeśli nie zbliżamy się → ignoruj
        if (closingSpeed <= 0.1f)
            continue;

        // =========================
        // TTC (Time To Collision)
        // =========================
        float ttc = dist / closingSpeed;

        // score = im mniejsze tym groźniejsze
        float score = ttc;

        if (score < bestScore)
        {
            bestScore = score;

            p.carAhead = o;
            p.hasCarAhead = true;
            p.distanceToCarAhead = dist;

            p.relativeSpeed =
                p.self.velocity.length() - o.velocity.length();

            p.relativeAcceleration =
                p.self.acceleration.length() - o.acceleration.length();
        }
    }
}