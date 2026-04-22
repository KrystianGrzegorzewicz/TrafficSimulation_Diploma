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

    buildCurve(); // 🔥 PRECOMPUTE
}
Vec2 Car::bezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) {
    float u = 1.0f - t;
    return p0 * (u * u) + p1 * (2 * u * t) + p2 * (t * t);
}
Vec2 Car::bezierDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) {
    return (p1 - p0) * (2 * (1 - t)) + (p2 - p1) * (2 * t);
}
void Car::buildCurve()
{
    curveSamples.clear();
    curveDistances.clear();

    if (travel.TravelPoints.size() < 3)
        return;

    const int RESOLUTION = 50; // im więcej, tym dokładniej

    totalLength = 0.0f;

    Vec2 prev;

    for (int i = 0; i < RESOLUTION; i++)
    {
        float t = (float)i / (RESOLUTION - 1);

        // segment Béziera
        Vec2 p0 = travel.TravelPoints[segment];
        Vec2 p1 = travel.TravelPoints[segment + 1];
        Vec2 p2 = travel.TravelPoints[segment + 2];

        Vec2 p = bezier(p0, p1, p2, t);

        curveSamples.push_back(p);

        if (i > 0)
        {
            float d = (p - prev).length();
            totalLength += d;
            curveDistances.push_back(totalLength);
        }

        prev = p;
    }
}
void Car::update(float dt)
{
    if (curveSamples.empty())
        return;

    traveled += speed * dt;

    if (traveled >= totalLength)
    {
        velocity = Vec2(0, 0);
        position = curveSamples.back();
        return;
    }

    // 🔥 znajdź indeks po długości
    size_t i = 0;
    while (i < curveDistances.size() && curveDistances[i] < traveled)
        i++;

    position = curveSamples[i];

    // 🔥 velocity z różnicy punktów
    if (i > 0)
    {
        Vec2 dir = curveSamples[i] - curveSamples[i - 1];
        float len = dir.length();

        if (len > 0.0001f)
            velocity = (dir / len) * speed;
        else
            velocity = Vec2(0, 0);
    }
}
Vec2 Car::getPosition() const {
    return position;
}

Vec2 Car::getVelocityVector() const {
    return velocity;
}