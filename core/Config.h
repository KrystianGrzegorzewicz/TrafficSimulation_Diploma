#pragma once

#include <print>
#include <iostream>

struct Config {
	bool gui;
	bool saveCsv;
	float simSpeed;
	float spawnRate;
	float AVRate;
	int junction;
	float aggressionBias;
	float aggressionSharpness;

	Config() :
		gui(true),
		simSpeed(5.0f),
		spawnRate(1.0f),
		AVRate(0.0f),
		junction(2),
		saveCsv(false),
		aggressionBias(0.5f),
		aggressionSharpness(0.5f)
	{
	}
};

static Config parseConfig()
{
	Config conf;
	int input = 1;

	std::print("Load default config? 1 - Yes, 0 - No: ");
	std::cin >> input;

	if (input != 1)
	{
		std::print("Connect to graphic interface? 1 - Yes, 0 - No: ");
		std::cin >> input;
		if (input == 0) conf.gui = false;

		std::print("Save data to csv? 1 - Yes, 0 - No: ");
		std::cin >> input;
		if (input == 1) conf.saveCsv = true;

		std::print("Set simulation speed (e.g. 1.0 for normal): ");
		std::cin >> conf.simSpeed;

		std::print("Set autonomous vehicle rate (0.0 - 1.0): ");
		std::cin >> conf.AVRate;

		std::print("Aggression bias (0.0 - 1.0): ");
		std::cin >> conf.aggressionBias;

		std::print("Aggression sharpness (0.0 - 1.0): ");
		std::cin >> conf.aggressionSharpness;

		std::print("Set junction sample (1, 2, 3, 4): ");
		std::cin >> conf.junction;
	}

	return conf;
}