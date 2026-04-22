#pragma once
#include "core/Lane.h"
#include "core/Travel.h"
#include <vector>

class Car {
private:
    std::vector<Lane*> route;
    float speed;
	Vec2 position;
	Travel travel;
	Vec2 velocity;
    int segment = 0;
    float t = 0.0f;
    std::vector<Vec2> curveSamples;
    std::vector<float> curveDistances;
    float totalLength = 0.0f;
    float traveled = 0.0f;

public:
    Car(float speed, Travel travel);
    Vec2 bezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
    Vec2 bezierDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
    void buildCurve();
    void update(float dt);

    Vec2 getPosition() const;
    Vec2 getVelocityVector() const;
};