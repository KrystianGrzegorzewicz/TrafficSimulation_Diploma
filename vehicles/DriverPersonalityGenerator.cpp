#include "DriverPersonalityGenerator.h"
#include <algorithm>

void DriverPersonalityGenerator::configure(
	float aggressionBias,
	float aggressionSharpness)
{
	bias =
		std::clamp(
			aggressionBias,
			0.0f,
			1.0f);

	sharpness =
		std::clamp(
			aggressionSharpness,
			0.0f,
			1.0f);
}

float DriverPersonalityGenerator::sampleAggression()
{
	float sigma =
		0.35f
		-
		sharpness * 0.30f;

	std::normal_distribution<float> dist(
		bias,
		sigma);

	return std::clamp(
		dist(rng),
		0.0f,
		1.0f);
}

DriverPersonality DriverPersonalityGenerator::generate()
{
	DriverPersonality p;

	p.aggression =
		sampleAggression();

	float a =
		p.aggression;

	p.accelFactor =
		0.7f + a * 0.6f;

	p.gapFactor =
		1.4f - a * 0.8f;

	p.reactionFactor =
		1.5f - a;

	p.startDelay =
		1.8f - a * 1.4f;

	p.curveFactor =
		0.6f + a * 0.4f;

	return p;
}