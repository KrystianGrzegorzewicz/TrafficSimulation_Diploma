#pragma once

#include "simulation/WorldState.h"
#include "vehicles/Car.h"
#include "road/Junction.h"
#include "core/Config.h"

#include <vector>
#include <memory>
#include <fstream>
#include <string>

class Simulation
{
public:
	Simulation(
		float spawnRate,
		int junctionIndex,
		bool saveCsv,
		float AVrate,
		const Config& conf
	);
	~Simulation();

	void step(float dt);
	std::string getWorldJson();
	int getFinishedVehicles() const { return finishedVehicles; }
	float getCurrentTime() const { return currentTime; }
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
	float csvAccumulator = 0.0f;
	const float csvPeriod = 0.1f;
	std::ofstream logFile;
	std::ofstream personalityFile;
	std::string outputDirectory;
	int finishedVehicles = 0;
};