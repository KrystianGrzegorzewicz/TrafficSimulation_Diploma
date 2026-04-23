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
        return;
    Vec2 p0 = travel.TravelPoints[segment];
    Vec2 p1 = travel.TravelPoints[segment + 1];
    Vec2 p2 = travel.TravelPoints[segment + 2];
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
        return;
    }

    float tLook = std::min(t + lookahead, 1.0f);
    Vec2 target = travel.bezier(p0, p1, p2, tLook);
    Vec2 tangent = travel.bezierDerivative(p0, p1, p2, tLook);
    float len = tangent.length();
    Vec2 forward = (len > 0.0001f) ? tangent / len : Vec2(0, 0);

    Vec2 toTarget = target - position;
    float forwardMag = toTarget.dot(forward);
    Vec2 forwardVec = forward * forwardMag;
    Vec2 lateralError = toTarget - forwardVec;
    float lateralVelMag = velocity.dot(Vec2(-forward.y, forward.x));
    Vec2 lateralVel = Vec2(-forward.y, forward.x) * lateralVelMag;

    Vec2 a_lateral = lateralError * kp - lateralVel * kd;
    float currentSpeed = velocity.length();
    float dynamicLookahead = lookaheadBase + speed * lookaheadSpeedFactor;

    float curveSpeed = travel.computeSpeedLimitAhead(
        segment,
        t,
        dynamicLookahead,
        aLatMax
    );

    float targetSpeed = std::min(maxSpeed, curveSpeed);
    float dv = targetSpeed - currentSpeed;
    float a_forward_scalar = 0.0f;
    if (dv > 0)
    {
        a_forward_scalar = dv * 2.0f;
    }
    else
    {
        a_forward_scalar = dv * 4.0f;
    }
    Vec2 a_forward = forward * a_forward_scalar;

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