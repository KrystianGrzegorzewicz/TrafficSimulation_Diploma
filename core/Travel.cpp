#include "core/Travel.h"
#include <iostream>

Travel::Travel(std::vector<Vec2> points, int w){
	Travel::TravelPoints = points;
	Travel::weight = w;
}
Vec2 Travel::bezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const {
    float u = 1.0f - t;
    return p0 * (u * u) + p1 * (2 * u * t) + p2 * (t * t);
}

Vec2 Travel::bezierDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const {
    return (p1 - p0) * (2 * (1 - t)) + (p2 - p1) * (2 * t);
}
float Travel::bezierCurvature(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const
{
    Vec2 d1 = bezierDerivative(p0, p1, p2, t);
    Vec2 d2 = (p2 - p1 * 2.0f + p0) * 2.0f;

    float numerator = fabs(d1.x * d2.y - d1.y * d2.x);
    float denom = pow(d1.x * d1.x + d1.y * d1.y, 1.5f);

    if (denom < 0.0001f) return 0.0f;
    return numerator / denom;
}
float Travel::bezierRadius(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t) const
{
    float k = bezierCurvature(p0, p1, p2, t);
    if (k < 0.00001f) return 999999.0f;
    return 1.0f / k;
}

float Travel::maxSpeedAt(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t, float aLatMax) const
{
    float R = bezierRadius(p0, p1, p2, t);
    return sqrt(aLatMax * R);
}
float Travel::computeSpeedLimitAhead(int segment, float t, float lookaheadT, float aLatMax) const
{
    float minSpeed = std::numeric_limits<float>::max();

    int seg = segment;
    float localT = t;

    const float STEP = 0.02f;

    float traveledT = 0.0f;

    while (traveledT < lookaheadT && seg + 2 < TravelPoints.size())
    {
        Vec2 p0 = TravelPoints[seg];
        Vec2 p1 = TravelPoints[seg + 1];
        Vec2 p2 = TravelPoints[seg + 2];

        float v = maxSpeedAt(p0, p1, p2, localT, aLatMax);
        minSpeed = std::min(minSpeed, v);

        localT += STEP;
        traveledT += STEP;

        if (localT >= 1.0f)
        {
            seg += 2;
            localT = 0.0f;
        }
    }
    if (minSpeed == std::numeric_limits<float>::max())
        return 50.0f;

    return minSpeed;
}
int Travel::getWeight() const { return weight; }