#pragma once

#include <string>

struct Config
{
	bool gui = 1;
	bool saveCsv = 0;

	float simSpeed = 10.0f;
	float spawnRate = 1.0f;
	float AVRate = 0.5f;
	std::string AVModel = "idm";

	int junction = 7;

	float aggressionBias = 0.5f;
	float aggressionSharpness = 0.5f;

	float maxSimulationTime = -1.0f;
	int maxVehiclesFinished = 10;

	int randomSeed = 0;
	int runId = 0;
	std::string experimentName = "";
};

Config parseConfig(int argc, char* argv[]);