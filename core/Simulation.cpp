#include "Simulation.h"
#include <sstream>
#include <random>
#include <chrono>
#include <iomanip>

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 1);

std::string getTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

Simulation::Simulation(float d, int junctionIndex, bool saveCsv) {
    this->thisJunction = TJunction(junctionIndex);
	timeAccumulator = 0.0f;
	period = 1.0f / d;
    cars.emplace_back(14.0f, thisJunction.getRandomTravel());
	blocks = thisJunction.getBlocks();
    if (saveCsv)
    {
        std::string filename = "data/cars_" + getTimestamp() + ".csv";
        logFile.open(filename);
        logFile << "time,id,travel_id,x,y,vx,vy,ax,ay\n";
    }    
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
    for (auto& block : blocks) {
		block.update(dt);
    }
    currentTime += dt;

    if (saveCsv)
		sendCsv();
    
    cars.erase(
        std::remove_if(cars.begin(), cars.end(),
            [](const Car& c) { return c.isFinished(); }),
        cars.end()
    );
}

void Simulation::sendCsv() {
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
		bool active = blocks[i].isActive();

        ss << "{\"x1\":" << topleft.x
            << ",\"y1\":" << topleft.y
            << ",\"x2\":" << bottomright.x
            << ",\"y2\":" << bottomright.y
            << ",\"active\":" << active << "}";
        if (i != blocks.size() - 1) ss << ",";
    }
    ss << "]}\n";
    return ss.str();
}