#pragma once
#pragma once

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
		AVRate(1.0f),
		junction(4),
		saveCsv(true),
		aggressionBias(0.5f),
		aggressionSharpness(0.5f)
	{
	}
};