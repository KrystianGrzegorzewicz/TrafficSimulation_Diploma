#pragma once
#pragma once
#include <vector>
#include <string>
#include "vehicles/Car.h"
#include "intersections/TJunction.h"
#include "core/Perception.h"
#include "core/Block.h"
#include <memory>

class Simulation {
private:
    float timeAccumulator;
	float period;
    std::vector<Car> cars;
    std::vector<Block> blocks;

	TJunction thisJunction;

public:
    Simulation(float d, int junctionIndex);
    void step(float dt);
    std::string getWorldJson();
};