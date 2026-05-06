#pragma once
#pragma once

struct Config {
	bool gui;
	bool saveCsv;
	float simSpeed;
	float spawnRate;
	int junction;

	Config() : gui(true), simSpeed(1.0f), spawnRate(1.0f), junction(4), saveCsv(false) {}
};