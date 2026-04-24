#include "vehicles/Car.h"
#include <cmath>
#include <algorithm>

Car::Car(float speed, Travel travel)
    : speed(speed), travel(travel)
{
    if (!travel.TravelPoints.empty())
        position = travel.TravelPoints[0];
    else
        position = Vec2(0, 0);

    velocity = Vec2(0, 0);
    acceleration = Vec2(0, 0);
}

void Car::update(float dt, const Perception& perception)
{
    if (travel.TravelPoints.size() < 3)
        return;

    if (segment + 2 >= travel.TravelPoints.size())
    {
        finished = true;
        velocity = Vec2(0, 0);
        return;
    }

    Vec2 oldVelocity = velocity;

    auto& p = travel.TravelPoints;

    Vec2 p0 = p[segment];
    Vec2 p1 = p[segment + 1];
    Vec2 p2 = p[segment + 2];

    // =========================
    // 1. TRACKING t
    // =========================
    float bestT = t;
    float bestDist = (travel.bezier(p0, p1, p2, t) - position).length();

    for (int i = -2; i <= 2; i++)
    {
        float testT = std::clamp(t + i * 0.02f, 0.0f, 1.0f);

        Vec2 pt = travel.bezier(p0, p1, p2, testT);
        float d = (pt - position).length();

        if (d < bestDist)
        {
            bestDist = d;
            bestT = testT;
        }
    }

    t = bestT;

    if (t >= 0.999f)
    {
        segment += 2;
        t = 0.0f;
        return;
    }

    // =========================
    // 2. BEHAVIOR (jedyny mózg)
    // =========================
    BehaviorOutput out = behavior.compute(
        travel,
        segment,
        t,
        velocity.length(),
        maxSpeed,
        maxAccel,
        maxDecel,
        lookaheadBase,
        lookaheadSpeedFactor,
        perception
    );

    // =========================
    // 3. KIERUNEK
    // =========================
    float lookahead = lookaheadBase + velocity.length() * lookaheadSpeedFactor;
    float tLook = std::min(t + lookahead, 1.0f);

    Vec2 tangent = travel.bezierDerivative(p0, p1, p2, tLook);
    Vec2 forward = tangent.normalized();

    // =========================
    // 4. LATERAL PD (skręt)
    // =========================
    Vec2 toTarget = out.targetPoint - position;

    float forwardMag = toTarget.dot(forward);
    Vec2 lateralError = toTarget - forward * forwardMag;

    Vec2 right(-forward.y, forward.x);

    float lateralVelMag = velocity.dot(right);
    Vec2 lateralVel = right * lateralVelMag;

    Vec2 a_lateral = lateralError * kp - lateralVel * kd;

    // =========================
    // 5. FINAL ACCELERATION
    // =========================
    acceleration = out.acceleration + a_lateral;

    float accLen = acceleration.length();
    if (accLen > maxAccel)
        acceleration = acceleration / accLen * maxAccel;

    // =========================
    // 6. INTEGRATION
    // =========================
    velocity += acceleration * dt;
    position += velocity * dt;

    speed = velocity.length();

    if (dt > 0.00001f)
        acceleration = (velocity - oldVelocity) / dt;
    else
        acceleration = Vec2(0, 0);
}


Vec2 Car::getPosition() const { return position; }
Vec2 Car::getVelocityVector() const { return velocity; }
Vec2 Car::getAccelerationVector() const { return acceleration; }
bool Car::isFinished() const { return finished; }