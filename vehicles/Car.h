#pragma once
#include "core/Lane.h"
#include <vector>

class Car {
private:
    std::vector<Lane*> route;
    int currentLaneIndex;
    float t;
    float speed;

public:
    Car(const std::vector<Lane*>& route, float startT, float speed);

    void update(float dt);

    Vec2 getPosition() const;
    Vec2 getVelocityVector() const;
};