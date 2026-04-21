#include "Simulation.h"
#include <sstream>
#include <random>

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 1);

Simulation::Simulation() {

    //roads.push_back(std::make_unique<TJunction>(450, 250));
    //const auto& lanes = roads[0]->getLanes();

    /*Lane* straight = (Lane*)&lanes[0];
    Lane* turn1 = (Lane*)&lanes[1];
    Lane* turn2 = (Lane*)&lanes[2];

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < 10; i++) {

        bool goStraight = dist(rng);

        if (goStraight) {
            std::vector<Lane*> route = { straight };
            cars.emplace_back(route, i * 0.1f, 80.0f, thisJunction.getRandomTravel());
        }
        else {
            std::vector<Lane*> route = { turn1, turn2 };
            cars.emplace_back(route, i * 0.1f, 80.0f, thisJunction.getRandomTravel());
        }
    }*/
    for (int i = 0; i < 10; i++) {
        cars.emplace_back( i * 0.2f, 80.0f, thisJunction.getRandomTravel());
    }
	blocks = thisJunction.getBlocks();
}

void Simulation::step(float dt) {
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

        ss << "{\"x\":" << pos.x
            << ",\"y\":" << pos.y
            << ",\"vx\":" << vel.x
            << ",\"vy\":" << vel.y << "}";

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