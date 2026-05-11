#pragma once
#include "vehicles/Car.h"
#include "road/Travel.h"
#include <memory>

// Factory — Simulation never constructs vehicles directly.
// Decouples concrete types from Simulation.
class VehicleFactory
{
public:
    static std::unique_ptr<Car> createHuman(
        float  initialSpeed,
        Travel travel
    );

    static std::unique_ptr<Car> createAV(
        float  initialSpeed,
        Travel travel
    );
};