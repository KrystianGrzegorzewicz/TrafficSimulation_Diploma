#pragma once
#include "core/Travel.h"
#include "core/BehaviorModel.h"
#include <vector>

class Car {
private:
    Travel travel;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float speed;

    // parametr na krzywej
    int segment = 0;
    float t = 0.0f;
    bool finished = false;

    // tuning
    float maxAccel = 6.0f;
    float maxDecel = 6.0f;
    float maxSpeed = 20.0f;

    float lookahead = 0.05f;
    float aLatMax = 6.0f;     // maks przyspieszenie boczne
    float lookaheadBase = 0.1f;
    float lookaheadSpeedFactor = 0.05f;

    // PD controller
    float kp = 8.0f;
    float kd = 4.0f;

    BehaviorModel behavior;

public:
    Car(float speed, Travel travel);
    ~Car() {};

    void update(float dt, const Perception& perception);

    Vec2 getPosition() const;
    Vec2 getVelocityVector() const;
    Vec2 getAccelerationVector() const;
    bool isFinished() const;
};