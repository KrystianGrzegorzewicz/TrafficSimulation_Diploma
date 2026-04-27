#include "core/Perception.h"
#include <limits>
#include <cmath>
#include <algorithm>

#define DEG2RAD (3.14159265359f / 180.0f)

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
        : Vec2(1, 0);

    Vec2 right(-forward.y, forward.x);

    // =========================
    // NOWE: dynamiczny FOV
    // =========================
    FOVResult fov = calculateFOV(p.self, forward, right);

    float fovDot = fov.fovDot;
    float maxViewDistance = fov.maxViewDistance;

    float laneWidth = 3.0f;

    for (const auto& o : others)
    {
        if ((o.position - p.self.position).length() < 0.001f)
            continue;

        Vec2 relPos = o.position - p.self.position;
        float dist = relPos.length();

        if (dist > maxViewDistance)
            continue;

        Vec2 dir = relPos / dist;

        // =========================
        // FOV
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

        if (closingSpeed <= 0.1f)
            continue;

        // =========================
        // TTC
        // =========================
        float ttc = dist / closingSpeed;
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

FOVResult calculateFOV(const CarState& self, const Vec2& forward, const Vec2& right)
{
    float speed = self.velocity.length();

    // =========================
    // SPEED FACTOR
    // =========================
    float speedT = std::clamp(speed / 15.0f, 0.0f, 1.0f);
    float speedFactor = speedT * speedT;

    // =========================
    // TURNING FACTOR
    // =========================
    float turningFactor = 0.0f;

    if (speed > 0.5f)
    {
        float lateralAcc = std::fabs(self.acceleration.dot(right));
        turningFactor = std::clamp(lateralAcc / 5.0f, 0.0f, 1.0f);
    }

    // =========================
    // FOV (kąt → dot)
    // =========================
    float minAngle = 50.0f * DEG2RAD;
    float maxAngle = 120.0f * DEG2RAD;

    float t = std::clamp(speedFactor - turningFactor * 0.7f, 0.0f, 1.0f);
    float angle = maxAngle - t * (maxAngle - minAngle);

    float fovDot = std::cos(angle);

    // =========================
    // VIEW DISTANCE
    // =========================
    float minView = 30.0f;
    float maxView = 80.0f;

    float viewT = std::clamp(speed / 20.0f, 0.0f, 1.0f);
    viewT = std::sqrt(viewT);

    float maxViewDistance = minView + viewT * (maxView - minView);

    return { fovDot, maxViewDistance };
}