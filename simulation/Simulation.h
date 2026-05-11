#pragma once
#include "simulation/WorldState.h"
#include "vehicles/Car.h"
#include "road/Junction.h"
#include <vector>
#include <memory>
#include <fstream>
#include <string>

// Simulation owns the update loop and time accounting.
// It does NOT know whether any Car is Human or AV.
class Simulation
{
public:
    Simulation(float spawnRate, int junctionIndex, bool saveCsv);
    ~Simulation();

    // Advance by dt seconds (called from main at fixed step).
    void step(float dt);

    std::string getWorldJson();
    void        sendCsv();

private:
    void spawnVehicle();
    void rebuildVehicleStates();   // fills worldState.vehicleStates before updates
    void pruneFinished();

    Junction  junction;
    WorldState worldState;

    std::vector<std::unique_ptr<Car>> vehicles;

    // Time book-keeping (moved out of main)
    float spawnAccumulator = 0.0f;
    float spawnPeriod = 1.0f;
    float currentTime = 0.0f;

    bool       saveCsv = false;
    std::ofstream logFile;
};