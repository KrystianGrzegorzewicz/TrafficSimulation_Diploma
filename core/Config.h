#pragma once

#include <string>

struct Config
{
	bool gui = true;
	bool saveCsv = true;

	float simSpeed = 7.0f;
	float spawnRate = 1.0f;
	float AVRate = 0.0f;

	int junction = 3;

	float aggressionBias = 0.5f;
	float aggressionSharpness = 0.5f;

	float maxSimulationTime = -1.0f;
	int maxVehiclesFinished = -1;

	int randomSeed = 0;
	int runId = 0;
	std::string experimentName = "";
};

Config parseConfig(int argc, char* argv[]);