#include "Simulation.h"
#include <sstream>
#include <random>

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 1);

Simulation::Simulation(float d) {
	timeAccumulator = 0.0f;
	period = 1.0f / d;
    //for (int i = 0; i < 10; i++) {
        cars.emplace_back(14.0f, thisJunction.getRandomTravel());
    //}
	blocks = thisJunction.getBlocks();
}

void Simulation::step(float dt) {
    timeAccumulator += dt;
    while (timeAccumulator >= period)
    {
        cars.emplace_back(14.0f, thisJunction.getRandomTravel());
        timeAccumulator -= period;
    }

    for (auto& car : cars)
        car.update(dt);
}

std::string Simulation::getWorldJson() {
    std::stringstream ss;
    ss << "{";

    // ===== CARS =====
    ss << "\"cars\":[";
    for (size_t i = 0; i < cars.size(); i++) {
        Vec2 pos = cars[i].getPosition();
        Vec2 vel = cars[i].getVelocityVector();
        Vec2 acc = cars[i].getAccelerationVector();

        ss << "{\"x\":" << pos.x
            << ",\"y\":" << pos.y
            << ",\"vx\":" << vel.x
            << ",\"vy\":" << vel.y
            << ",\"ax\":" << acc.x
            << ",\"ay\":" << acc.y << "}";

        if (i != cars.size() - 1) ss << ",";
    }
    ss << "],";
    // ===== BLOCKS =====
	ss << "\"blocks\":[";
    for (size_t i = 0; i < blocks.size(); i++)
    {
		Vec2 topleft = blocks[i].getTopLeft();
		Vec2 bottomright = blocks[i].getBottomRight();

        ss << "{\"x1\":" << topleft.x
            << ",\"y1\":" << topleft.y
            << ",\"x2\":" << bottomright.x
            << ",\"y2\":" << bottomright.y << "}";
        if (i != blocks.size() - 1) ss << ",";
    }
    ss << "]}\n";
    return ss.str();
}