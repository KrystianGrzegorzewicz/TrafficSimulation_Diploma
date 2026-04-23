#include "vehicles/Car.h"
#include <cmath>
#include <algorithm>

Car::Car(float speed, Travel travel)
    : speed(speed), travel(travel)
{
    if (!travel.TravelPoints.empty())
        position = travel.TravelPoints[0];
    else
        position = Vec2(0, 0);

    velocity = Vec2(0, 0);
    acceleration = Vec2(0, 0);
}

// ================= UPDATE =================

void Car::update(float dt)
{
    if (travel.TravelPoints.size() < 3)
        return;

    Vec2 oldVelocity = velocity;

    // --- aktualny segment ---
    if (segment + 2 >= travel.TravelPoints.size())
        return;

    Vec2 p0 = travel.TravelPoints[segment];
    Vec2 p1 = travel.TravelPoints[segment + 1];
    Vec2 p2 = travel.TravelPoints[segment + 2];

    // =========================
    // 1. znajdź lepsze t (lokalne dopasowanie)
    // =========================
    float bestT = t;
    float bestDist = (travel.bezier(p0, p1, p2, t) - position).length();

    // lokalne szukanie zamiast samplowania całej krzywej
    for (int i = -2; i <= 2; i++)
    {
        float testT = t + i * 0.02f;
        testT = std::clamp(testT, 0.0f, 1.0f);

        Vec2 pt = travel.bezier(p0, p1, p2, testT);
        float d = (pt - position).length();

        if (d < bestDist)
        {
            bestDist = d;
            bestT = testT;
        }
    }

    t = bestT;

    // przejście do następnego segmentu
    if (t >= 0.999f)
    {
        segment += 2;
        t = 0.0f;
        return;
    }

    // =========================
    // 2. lookahead
    // =========================
    float tLook = std::min(t + lookahead, 1.0f);

    Vec2 target = travel.bezier(p0, p1, p2, tLook);
    Vec2 tangent = travel.bezierDerivative(p0, p1, p2, tLook);

    float len = tangent.length();
    Vec2 forward = (len > 0.0001f) ? tangent / len : Vec2(0, 0);

    // =========================
    // 3. błąd boczny
    // =========================
    Vec2 toTarget = target - position;

    // rozkład na forward + lateral
    float forwardMag = toTarget.dot(forward);
    Vec2 forwardVec = forward * forwardMag;

    Vec2 lateralError = toTarget - forwardVec;

    // prędkość boczna
    float lateralVelMag = velocity.dot(Vec2(-forward.y, forward.x));
    Vec2 lateralVel = Vec2(-forward.y, forward.x) * lateralVelMag;

    // =========================
    // 4. PD sterowanie
    // =========================
    Vec2 a_lateral = lateralError * kp - lateralVel * kd;

    // forward (gaz)
    float currentSpeed = velocity.length();
    // =========================
    // LOOKAHEAD zależny od prędkości
    // =========================
    float dynamicLookahead = lookaheadBase + speed * lookaheadSpeedFactor;

    // =========================
    // SPEED PREVIEW
    // =========================
    float curveSpeed = travel.computeSpeedLimitAhead(
        segment,
        t,
        dynamicLookahead,
        aLatMax
    );

    // finalny target
    float targetSpeed = std::min(maxSpeed, curveSpeed);
    float dv = targetSpeed - currentSpeed;

    float a_forward_scalar = 0.0f;

    if (dv > 0)
    {
        // przyspieszanie
        a_forward_scalar = dv * 2.0f;
    }
    else
    {
        // hamowanie mocniejsze
        a_forward_scalar = dv * 4.0f;
    }
    Vec2 a_forward = forward * a_forward_scalar;

    // =========================
    // 5. suma
    // =========================
    acceleration = a_forward + a_lateral;

    // limit przyspieszenia
    float accLen = acceleration.length();
    if (accLen > maxAccel)
        acceleration = acceleration / accLen * maxAccel;

    // =========================
    // 6. integracja fizyki
    // =========================
    velocity += acceleration * dt;
    position += velocity * dt;

    // aktualizacja speed
    speed = velocity.length();

    // =========================
    // 7. final accel (debug)
    // =========================
    if (dt > 0.00001f)
        acceleration = (velocity - oldVelocity) / dt;
    else
        acceleration = Vec2(0, 0);
}

// ================= GETTERY =================

Vec2 Car::getPosition() const {
    return position;
}

Vec2 Car::getVelocityVector() const {
    return velocity;
}

Vec2 Car::getAccelerationVector() const {
    return acceleration;
}