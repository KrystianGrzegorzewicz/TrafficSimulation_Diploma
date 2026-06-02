#pragma once

#include "vehicles/DriverPersonality.h"
#include <random>

class DriverPersonalityGenerator
{
public:

	static void configure(
		float aggressionBias,
		float aggressionSharpness
	);

	static DriverPersonality generate();

private:

	static float sampleAggression();

	static inline float bias = 0.5f;
	static inline float sharpness = 0.5f;

	static inline std::mt19937 rng{ std::random_device{}() };
};