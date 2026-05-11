#include "core/Simulation.h"
#include "core/BlockPerception.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

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
    this->saveCsv = saveCsv;
    
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

void Simulation::updateBlocks(float dt)
{
    for (auto& block : blocks)
    {
        block.update(dt);
    }
}

void Simulation::step(float dt) {
    timeAccumulator += dt;
    while (timeAccumulator >= period)
    {
        cars.emplace_back(14.0f, thisJunction.getRandomTravel());
        timeAccumulator -= period;
    }

    // Update block states (on/off cycling)
    updateBlocks(dt);

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
        
        // Update block perception
        BlockPerception::updateBlockPerception(blocks, p.self, p.blockHazard);
        p.hasBlockHazard = (p.blockHazard.blockIndex >= 0);

        cars[i].update(dt, p);
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
        
        if (i < cars.size() - 1)
            ss << ",";
    }
    ss << "]";
    
    ss << ",\"blocks\":[";
    for (size_t i = 0; i < blocks.size(); i++) {
        Vec2 topLeft = blocks[i].getTopLeft();
        Vec2 bottomRight = blocks[i].getBottomRight();
		bool active = blocks[i].isActive();
        //test
        ss << "{\"x1\":" << topLeft.x
            << ",\"y1\":" << topLeft.y
            << ",\"x2\":" << bottomRight.x
            << ",\"y2\":" << bottomRight.y
            << ",\"active\":" << active  << "}";
        if (i != blocks.size() - 1) ss << ",";
    }
    ss << "]";
    
    // Serialize lines
    ss << ",\"lines\":[";
    std::vector<Line> lines = thisJunction.getLines();
    for (size_t i = 0; i < lines.size(); i++) {
        Vec2 start = lines[i].getStart();
        Vec2 end = lines[i].getEnd();
        float thickness = lines[i].getThickness();
        
        ss << "{\"x1\":" << start.x
            << ",\"y1\":" << start.y
            << ",\"x2\":" << end.x
            << ",\"y2\":" << end.y
            << ",\"thickness\":" << thickness
            << ",\"r\":" << lines[i].getColor(0)
            << ",\"g\":" << lines[i].getColor(1)
            << ",\"b\":" << lines[i].getColor(2) << "}";
        if (i != lines.size() - 1) ss << ",";
    }
    ss << "]";
    
    // Serialize circles
    ss << ",\"circles\":[";
    std::vector<Circle> circles = thisJunction.getCircles();
    for (size_t i = 0; i < circles.size(); i++) {
        Vec2 center = circles[i].getCenter();
        float radius = circles[i].getRadius();
        bool filled = circles[i].isFilled();
        
        ss << "{\"cx\":" << center.x
            << ",\"cy\":" << center.y
            << ",\"radius\":" << radius
            << ",\"filled\":" << (filled ? "true" : "false")
            << ",\"r\":" << circles[i].getColor(0)
            << ",\"g\":" << circles[i].getColor(1)
            << ",\"b\":" << circles[i].getColor(2) << "}";
        if (i != circles.size() - 1) ss << ",";
    }
    ss << "]";
    
    ss << "}\n";
    return ss.str();
}
