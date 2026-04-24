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

void Car::update(float dt)
{
    if (travel.TravelPoints.size() < 3)
        return;

    Vec2 oldVelocity = velocity;

    if (segment + 2 >= travel.TravelPoints.size())
    {
        finished = true;
        velocity = Vec2(0, 0);
        return;
    }

    auto& p = travel.TravelPoints;

    Vec2 p0 = p[segment];
    Vec2 p1 = p[segment + 1];
    Vec2 p2 = p[segment + 2];

    // =========================
    // znajdź t (BEZ ZMIAN)
    // =========================
    float bestT = t;
    float bestDist = (travel.bezier(p0, p1, p2, t) - position).length();

    for (int i = -2; i <= 2; i++)
    {
        float testT = t + i * 0.02f;
        testT = std::clamp(testT, 0.0f, 1.0f);

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

        if (segment + 2 >= travel.TravelPoints.size())
        {
            finished = true;
            velocity = Vec2(0, 0);
        }

        return;
    }

    // =========================
    // BEHAVIOR (NOWE)
    // =========================
    BehaviorOutput out = behavior.compute(
        travel,
        segment,
        t,
        velocity.length(),
        maxSpeed,
        aLatMax,
        lookaheadBase,
        lookaheadSpeedFactor
    );

    // =========================
    // kierunek z krzywej (jak było)
    // =========================
    float dynamicLookahead = lookaheadBase + velocity.length() * lookaheadSpeedFactor;
    float tLook = std::min(t + dynamicLookahead, 1.0f);
    Vec2 tangent = travel.bezierDerivative(p0, p1, p2, tLook);
    Vec2 forward = tangent.length() > 0.0001f
        ? tangent / tangent.length()
        : Vec2(1, 0);

    // =========================
    // lateral control (jak było)
    // =========================
    Vec2 toTarget = out.targetPoint - position;

    float forwardMag = toTarget.dot(forward);
    Vec2 forwardVec = forward * forwardMag;

    Vec2 lateralError = toTarget - forwardVec;

    float lateralVelMag = velocity.dot(Vec2(-forward.y, forward.x));
    Vec2 lateralVel = Vec2(-forward.y, forward.x) * lateralVelMag;

    Vec2 a_lateral = lateralError * kp - lateralVel * kd;

    // =========================
    // forward (z behavior)
    // =========================
    float currentSpeed = velocity.length();
    float dv = out.targetSpeed - currentSpeed;

    float a_forward_scalar = (dv > 0) ? dv * 2.0f : dv * 4.0f;
    Vec2 a_forward = forward * a_forward_scalar;

    // =========================
    // suma
    // =========================
    acceleration = a_forward + a_lateral;

    float accLen = acceleration.length();
    if (accLen > maxAccel)
        acceleration = acceleration / accLen * maxAccel;

    velocity += acceleration * dt;
    position += velocity * dt;
    speed = velocity.length();

    if (dt > 0.00001f)
        acceleration = (velocity - oldVelocity) / dt;
    else
        acceleration = Vec2(0, 0);
}

Vec2 Car::getPosition() const {
    return position;
}
Vec2 Car::getVelocityVector() const {
    return velocity;
}
Vec2 Car::getAccelerationVector() const {
    return acceleration;
}
bool Car::isFinished() const { return finished; }