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
    Vec2 acceleration;
    int segment = 0;
    float t = 0.0f;
    std::vector<Vec2> curveSamples;
    std::vector<float> curveDistances;
    float totalLength = 0.0f;
    float traveled = 0.0f;
    float desiredSpeed = 0.0f;
    float maxAccel = 4.0f;


public:
    Car(float speed, Travel travel);
    Vec2 bezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
    Vec2 bezierDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
    Vec2 bezierSecondDerivative(const Vec2& p0, const Vec2& p1, const Vec2& p2);
    void buildCurve();
    float bezierCurvature(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
    float bezierRadius(const Vec2& p0, const Vec2& p1, const Vec2& p2, float t);
    int currentBezierSegment(size_t sampleIndex);
    float maxSpeedFromRadius(float R);
    float segmentSpeedLimit(int segIndex);
    float computeDesiredSpeed(size_t currentIndex);
    void accelerate(float dt, size_t curveIndex);
    float brakingDistance(float v, float targetV);

    void update(float dt);

    Vec2 getPosition() const;
    Vec2 getVelocityVector() const;
    Vec2 getAccelerationVector() const;
};