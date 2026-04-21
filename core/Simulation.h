#pragma once
#pragma once
#include <vector>
#include <string>
#include "vehicles/Car.h"
#include "intersections/TJunction.h"
#include "core/Block.h"
#include <memory>

class Simulation {
private:
    std::vector<Car> cars;
    std::vector<std::unique_ptr<Road>> roads;
    std::vector<Block> blocks;

public:
    Simulation();
    void step(float dt);
    std::string getWorldJson();
};