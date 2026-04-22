#include "vehicles/Car.h"
#include <iostream>

Car::Car(float speed, Travel travel)
	: speed(speed), travel(travel), currentTarget(0)
{
    if (!travel.TravelPoints.empty()) {
        position = travel.TravelPoints[0];
    }
    else {
        position = Vec2(0, 0);
    }
}

void Car::update(float dt) {
    if (currentTarget >= travel.TravelPoints.size()) {
        velocity = Vec2(0, 0);
        return;
    }

    Vec2 target = travel.TravelPoints[currentTarget];
    Vec2 toTarget = target - position;

    float dist = toTarget.length();

    if (dist < 0.001f) {
        currentTarget++;
        if (currentTarget >= travel.TravelPoints.size()) {
            velocity = Vec2(0, 0);
            return;
        }
        target = travel.TravelPoints[currentTarget];
        toTarget = target - position;
        dist = toTarget.length();
    }

    Vec2 dir = toTarget / dist;
    velocity = dir * speed;

    float move = speed * dt;

    if (move >= dist) {
        // nie przeskakuj punktu
        position = target;
        currentTarget++;
        if (currentTarget >= travel.TravelPoints.size()) {
            velocity = Vec2(0, 0);
            return;
        }
    }
    else {
        position = position + dir * move;
    }
}

Vec2 Car::getPosition() const {
    return position;
}

Vec2 Car::getVelocityVector() const {
    if (currentTarget >= travel.TravelPoints.size())
        return Vec2(0, 0);

    Vec2 target = travel.TravelPoints[currentTarget];
    Vec2 dir = target - position;

    float len = dir.length();
    if (len < 0.0001f)
        return Vec2(0, 0);

    return (dir / len) * speed;
}