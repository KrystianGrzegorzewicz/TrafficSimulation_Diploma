#include "vehicles/Car.h"

Car::Car(const std::vector<Lane*>& r, float startT, float s)
    : route(r), currentLaneIndex(0), t(startT), speed(s) {
}

void Car::update(float dt) {

    Lane* lane = route[currentLaneIndex];

    float distance = speed * dt;
    float deltaT = distance / lane->getLength();
    t += deltaT;

    // koniec pasa → przejdź do następnego
    if (t >= 1.0f) {
        t = 0.0f;
        currentLaneIndex++;

        // koniec trasy → restart auta
        if (currentLaneIndex >= route.size())
            currentLaneIndex = 0;
    }
}

Vec2 Car::getPosition() const {
    return route[currentLaneIndex]->getPoint(t);
}

Vec2 Car::getVelocityVector() const {
    Vec2 dir = route[currentLaneIndex]->getDirection();
    return dir * speed;
}