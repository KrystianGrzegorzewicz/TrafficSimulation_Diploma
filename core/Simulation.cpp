#include "Simulation.h"
#include <sstream>
#include <random>

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 1);

Simulation::Simulation(float d, int junctionIndex) {
    this->thisJunction = TJunction(junctionIndex);
	timeAccumulator = 0.0f;
	period = 1.0f / d;
    cars.emplace_back(14.0f, thisJunction.getRandomTravel());
	blocks = thisJunction.getBlocks();
    logFile.open("data/cars.csv");
    logFile << "time,id,travel_id,x,y,vx,vy,ax,ay\n";
}
Simulation::~Simulation() {
    if (logFile.is_open())
        logFile.close();
}

void Simulation::step(float dt) {
    timeAccumulator += dt;
    while (timeAccumulator >= period)
    {
        cars.emplace_back(14.0f, thisJunction.getRandomTravel());
        timeAccumulator -= period;
    }

    std::vector<CarState> states;
    states.reserve(cars.size());

    for (const auto& car : cars)
    {
        states.push_back({
            car.getPosition(),
            car.getVelocityVector(),
            car.getAccelerationVector()
            });
    }

    for (size_t i = 0; i < cars.size(); i++)
    {
        Perception p;
        p.self = states[i];

        updatePerception(p, states);

        cars[i].update(dt, p);
    }
    currentTime += dt;

    for (const auto& car : cars)
    {
        Vec2 pos = car.getPosition();
        Vec2 vel = car.getVelocityVector();
        Vec2 acc = car.getAccelerationVector();

        logFile << currentTime << ","
            << car.getId() << ","
            << car.getTravelId() << ","
            << pos.x << "," << pos.y << ","
            << vel.x << "," << vel.y << ","
            << acc.x << "," << acc.y << "\n";
    }

    cars.erase(
        std::remove_if(cars.begin(), cars.end(),
            [](const Car& c) { return c.isFinished(); }),
        cars.end()
    );
}

std::string Simulation::getWorldJson() {
    std::stringstream ss;
    ss << "{";

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