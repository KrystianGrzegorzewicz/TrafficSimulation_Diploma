#include "vehicles/Car.h"
#include <iostream>

Car::Car(float speed, Travel travel)
    : speed(speed), travel(travel)
{
    if (!travel.TravelPoints.empty())
        position = travel.TravelPoints[0];
    else
        position = Vec2(0, 0);

    segment = 0;
    t = 0.0f;
    traveled = 0.0f;

    buildCurve();
}
Vec2 Car::bezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) {
    float u = 1.0f - t;
    return p0 * (u * u) + p1 * (2 * u * t) + p2 * (t * t);
}
Vec2 Car::bezierDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) {
    return (p1 - p0) * (2 * (1 - t)) + (p2 - p1) * (2 * t);
}
Vec2 Car::bezierSecondDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2) {
    return (p2 - p1 * 2.0f + p0) * 2.0f;
}
void Car::buildCurve()
{
    curveSamples.clear();
    curveDistances.clear();
    totalLength = 0.0f;

    if (travel.TravelPoints.size() < 3)
        return;

    const int RESOLUTION = 50;

    Vec2 prev;
    bool firstPoint = true;

    for (size_t s = 0; s + 2 < travel.TravelPoints.size(); s += 2)
    {
        Vec2 p0 = travel.TravelPoints[s];
        Vec2 p1 = travel.TravelPoints[s + 1];
        Vec2 p2 = travel.TravelPoints[s + 2];

        for (int i = 0; i < RESOLUTION; i++)
        {
            float t = (float)i / (RESOLUTION - 1);

            Vec2 p = bezier(p0, p1, p2, t);

            if (!firstPoint && i == 0)
                continue;

            curveSamples.push_back(p);

            if (!firstPoint)
            {
                float d = (p - prev).length();
                totalLength += d;
                curveDistances.push_back(totalLength);
            }

            prev = p;
            firstPoint = false;
        }
    }
}
float Car::bezierCurvature(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t)
{
    Vec2 d1 = bezierDerivative(p0, p1, p2, t);
    Vec2 d2 = bezierSecondDerivative(p0, p1, p2);

    float numerator = fabs(d1.x * d2.y - d1.y * d2.x);
    float denom = pow(d1.x * d1.x + d1.y * d1.y, 1.5f);

    if (denom < 0.0001f) return 0.0f;
    return numerator / denom;
}
float Car::bezierRadius(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t)
{
    float k = bezierCurvature(p0, p1, p2, t);
    if (k < 0.00001f) return 999999.0f;
    return 1.0f / k;
}
int Car::currentBezierSegment(size_t sampleIndex)
{
    const int RESOLUTION = 50;
    return (sampleIndex / RESOLUTION) * 2;
}
float Car::maxSpeedFromRadius(float R)
{
    const float aLatMax = 4.0f;
    return sqrt(aLatMax * R);
}
float Car::segmentSpeedLimit(int segIndex)
{
    Vec2 p0 = travel.TravelPoints[segIndex];
    Vec2 p1 = travel.TravelPoints[segIndex + 1];
    Vec2 p2 = travel.TravelPoints[segIndex + 2];

    const int SAMPLES = 20; // wystarczy!

    float minSpeed = 9999.0f;

    for (int i = 0; i < SAMPLES; i++)
    {
        float t = (float)i / (SAMPLES - 1);

        float R = bezierRadius(p0, p1, p2, t);
        float vmax = maxSpeedFromRadius(R);

        minSpeed = std::min(minSpeed, vmax);
    }

    return minSpeed;
}
float Car::computeDesiredSpeed(size_t currentSampleIndex)
{
    float distAhead = 0.0f;

    int startSeg = currentBezierSegment(currentSampleIndex);

    // patrzymy tylko na drogę PRZED autem
    for (int seg = startSeg; seg + 2 < travel.TravelPoints.size(); seg += 2)
    {
        float segLength = 0.0f;

        Vec2 p0 = travel.TravelPoints[seg];
        Vec2 p1 = travel.TravelPoints[seg + 1];
        Vec2 p2 = travel.TravelPoints[seg + 2];

        Vec2 prev = bezier(p0, p1, p2, 0.0f);

        const int SAMPLES = 120;

        for (int i = 1; i < SAMPLES; i++)
        {
            float t = (float)i / (SAMPLES - 1);
            Vec2 p = bezier(p0, p1, p2, t);
            segLength += (p - prev).length();
            prev = p;
        }

        distAhead += segLength;

        float curveSpeed = segmentSpeedLimit(seg);
        float brakeDist = brakingDistance(speed, curveSpeed);

        if (brakeDist >= distAhead)
            return curveSpeed;
    }

    return 999.0f;
}
void Car::accelerate(float dt, size_t curveIndex)
{
    desiredSpeed = computeDesiredSpeed(curveIndex);

    float dv = desiredSpeed - speed;

    float accel = 0.0f;

    if (dv > 0)
        accel = maxAccel;      // gaz
    else
        accel = -maxAccel;     // hamulec

    speed += accel * dt;

    if (speed < 0) speed = 0;
}
float Car::brakingDistance(float v, float targetV)
{
    if (v <= targetV) return 0.0f;
    return (v * v - targetV * targetV) / (2.0f * maxAccel);
}

void Car::update(float dt)
{
    Vec2 oldVelocity = velocity;
    if (curveSamples.empty())
        return;

    traveled += speed * dt;

    if (traveled >= totalLength)
    {
        velocity = Vec2(0, 0);
        position = curveSamples.back();
        return;
    }

    size_t i = 0;
    while (i < curveDistances.size() && curveDistances[i] < traveled)
        i++;
    accelerate(dt, i);

    position = curveSamples[i];

    if (i > 0)
    {
        Vec2 dir = curveSamples[i] - curveSamples[i - 1];
        float len = dir.length();

        if (len > 0.0001f)
            velocity = (dir / len) * speed;
        else
            velocity = Vec2(0, 0);
    }
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
Vec2 Car::getAccelerationVector() const
{
    return acceleration;
}