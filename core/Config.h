#pragma once

#include <string>

struct Config
{
	bool gui = 1;
	bool saveCsv = 1;

	float simSpeed = 1.0f;
	float spawnRate = 0.50f;
	float AVRate = 0.0f;

	int junction = 5;

	float aggressionBias = 0.5f;
	float aggressionSharpness = 0.5f;

	float maxSimulationTime = -1.0f;
	int maxVehiclesFinished = -1;

	int randomSeed = 0;
	int runId = 0;
	std::string experimentName = "";
};

Config parseConfig(int argc, char* argv[]);