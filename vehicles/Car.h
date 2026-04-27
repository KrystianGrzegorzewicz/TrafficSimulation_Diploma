#pragma once

#include "core/Travel.h"
#include "core/BehaviorModel.h"
#include "core/Perception.h"

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
    // =========================
    // PATH
    // =========================
    Travel travel;
    int segment = 0;
    float t = 0.0f;
    bool finished = false;

    // =========================
    // KINEMATICS
    // =========================
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    float speed;

    // =========================
    // TUNING
    // =========================
    float maxAccel = 6.0f;
    float maxDecel = 6.0f;
    float maxSpeed = 20.0f;

    float lookaheadBase = 0.1f;
    float lookaheadSpeedFactor = 0.03f;

    // PD controller
    float kp = 8.0f;
    float kd = 4.0f;

    BehaviorModel behavior;

private:
    // =========================
    // UPDATE STEPS
    // =========================
    bool isPathValid() const;
    bool isFinishedInternal();

    void updateClosestT();
    bool advanceSegmentIfNeeded();

    Vec2 computeLateralAcceleration(
        const BehaviorOutput& behaviorOut
    );

    void integrate(
        const Vec2& desiredAcceleration,
        float dt
    );
};