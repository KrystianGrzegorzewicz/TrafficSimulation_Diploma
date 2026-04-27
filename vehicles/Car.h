#pragma once

#include "core/Travel.h"
#include "core/BehaviorModel.h"
#include "core/Perception.h"
#include "core/SteeringModel.h"

class Car
{
public:
    Car(float speed, Travel travel);
    ~Car() = default;

    void update(float dt, const Perception& perception);

    Vec2 getPosition() const;
    Vec2 getVelocityVector() const;
    Vec2 getAccelerationVector() const;
    bool isFinished() const;

private:
    Travel travel;
    int segment = 0;
    float t = 0.0f;
    bool finished = false;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    float speed;

	//tuning parameters
    float maxAccel = 6.0f;
    float maxDecel = 6.0f;
    float maxSpeed = 20.0f;

    float lookaheadBase = 0.1f;
    float lookaheadSpeedFactor = 0.03f;

    // PD controller
    float kp = 8.0f;
    float kd = 4.0f;

    SteeringModel steering{ kp, kd };
    BehaviorModel behavior;

private:
    bool isPathValid() const;
    bool isFinishedInternal();

    void updateClosestT();
    bool advanceSegmentIfNeeded();

    void integrate(
        const Vec2& desiredAcceleration,
        float dt
    );
};