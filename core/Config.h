#pragma once

#include <string>

struct Config
{
	bool gui = 1;
	bool saveCsv = 1;

	float simSpeed = 6.0f;
	float spawnRate = 0.7f;
	float AVRate = 1.0f;

	int junction = 4;

	float aggressionBias = 0.5f;
	float aggressionSharpness = 0.2f;

	float maxSimulationTime = -1.0f;
	int maxVehiclesFinished = 20;

	int randomSeed = 0;
	int runId = 0;
	std::string experimentName = "";
};

Config parseConfig(int argc, char* argv[]);