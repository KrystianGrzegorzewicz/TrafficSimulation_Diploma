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
		simSpeed(1.0f),
		spawnRate(0.001f),
		AVRate(1.0f),
		junction(1),
		saveCsv(true),
		aggressionBias(0.5f),
		aggressionSharpness(0.5f)
	{
	}
};