#pragma once
#include "core/Lane.h"
#include "core/Travel.h"
#include <vector>

class Car {
private:
    std::vector<Lane*> route;
    Travel travel;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float speed;

    // parametr na krzywej
    int segment = 0;
    float t = 0.0f;

    // tuning
    float maxAccel = 6.0f;
    float maxSpeed = 20.0f;

    float lookahead = 0.05f;
    float aLatMax = 6.0f;     // maks przyspieszenie boczne
    float lookaheadBase = 0.1f;
    float lookaheadSpeedFactor = 0.03f;

    // PD controller
    float kp = 8.0f;
    float kd = 4.0f;

public:
    Car(float speed, Travel travel);

    void update(float dt);

    Vec2 getPosition() const;
    Vec2 getVelocityVector() const;
    Vec2 getAccelerationVector() const;
};