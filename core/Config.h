#pragma once
#pragma once

struct Config {
	bool gui;
	float simSpeed;
	float spawnRate;

	Config() : gui(true), simSpeed(1.0f), spawnRate(1.0f) {}
};