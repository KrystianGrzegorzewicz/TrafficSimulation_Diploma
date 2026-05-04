#include "vehicles/Car.h"
#include <algorithm>
#include <cmath>

int Car::nextId = 0;

Car::Car(float speed, Travel travel)
    : speed(speed), travel(travel)
{
    if (!travel.TravelPoints.empty())
        position = travel.TravelPoints[0];
    else
        position = Vec2(0, 0);

    velocity = Vec2(0, 0);
    acceleration = Vec2(0, 0);
    id = nextId++;
    travelId = travel.getId();
}

void Car::update(float dt, const Perception& perception)
{
    if (!isPathValid())
        return;

    if (isFinishedInternal())
        return;

    updateClosestT();

    if (advanceSegmentIfNeeded())
        return;

    BehaviorOutput behaviorOut = behavior.compute(
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

    Vec2 lateralAccel =
        steering.computeLateralAcceleration(
            travel,
            segment,
            t,
            position,
            velocity,
            lookaheadBase,
            lookaheadSpeedFactor,
            behaviorOut
        );

    integrate(
        behaviorOut.acceleration + lateralAccel,
        dt
    );
}

bool Car::isPathValid() const
{
    return travel.TravelPoints.size() >= 3;
}

bool Car::isFinishedInternal()
{
    if (segment + 2 >= travel.TravelPoints.size())
    {
        finished = true;
        velocity = Vec2(0, 0);
        return true;
    }
    return false;
}

void Car::updateClosestT()
{
    auto& p = travel.TravelPoints;

    Vec2 p0 = p[segment];
    Vec2 p1 = p[segment + 1];
    Vec2 p2 = p[segment + 2];

    float bestT = t;
    float bestDist =
        (travel.bezier(p0, p1, p2, t) - position).length();

    for (int i = -2; i <= 2; i++)
    {
        float testT =
            std::clamp(t + i * 0.02f, 0.0f, 1.0f);

        float d =
            (travel.bezier(p0, p1, p2, testT) - position).length();

        if (d < bestDist)
        {
            bestDist = d;
            bestT = testT;
        }
    }

    t = bestT;
}

bool Car::advanceSegmentIfNeeded()
{
    if (t >= 0.999f)
    {
        segment += 2;
        t = 0.0f;
        return true;
    }
    return false;
}

void Car::integrate(const Vec2& desiredAcceleration, float dt)
{
    Vec2 oldVelocity = velocity;

    acceleration = desiredAcceleration;

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

int Car::getId() const { return id; }
int Car::getTravelId() const { return travelId; }
Vec2 Car::getPosition() const { return position; }
Vec2 Car::getVelocityVector() const { return velocity; }
Vec2 Car::getAccelerationVector() const { return acceleration; }
bool Car::isFinished() const { return finished; }