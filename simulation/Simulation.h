#pragma once

#include "simulation/WorldState.h"
#include "vehicles/Car.h"
#include "road/Junction.h"

#include <vector>
#include <memory>
#include <fstream>
#include <string>

class Simulation
{
public:
	Simulation(float spawnRate, int junctionIndex, bool saveCsv, const float AVrate);
	~Simulation();

	void step(float dt);
	std::string getWorldJson();
	void sendCsv();

private:
	void spawnVehicle();
	void rebuildVehicleStates();
	void pruneFinished();

	Junction junction;
	WorldState worldState;
	std::vector<std::unique_ptr<Car>> vehicles;

	float spawnAccumulator = 0.0f;
	float spawnPeriod = 1.0f;
	float currentTime = 0.0f;
	float AVrate = 0.0f;

	bool saveCsv = false;
	std::ofstream logFile;
	std::ofstream personalityFile;
};