#pragma once
#include "core/Lane.h"
#include "core/Travel.h"
#include <vector>

class Car {
private:
    std::vector<Lane*> route;
    int currentTarget;
    float t;
    float speed;
	Vec2 position;
	Travel travel;
	Vec2 velocity;

public:
    Car(float startT, float speed, Travel travel);

    void update(float dt);

    Vec2 getPosition() const;
    Vec2 getVelocityVector() const;
};