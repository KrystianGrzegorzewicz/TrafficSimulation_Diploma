#include "behavior/longitudinal/IDMLongitudinalModel.h"
#include <algorithm>
#include <cmath>

float IDMLongitudinalModel::computeAcceleration(
	const CarState& self,
	const PerceptionState& perception,
	float desiredSpeed,
	float maxAccel,
	float maxDecel
)
{
	constexpr float delta = 4.0f;
	float s0 = 3.0f * gapFactor;
	float T = 1.2f * gapFactor;

	float v = self.velocity.length();

	if (desiredSpeed < 0.001f)
		return -maxDecel;

	float freeRoad = std::pow(v / desiredSpeed, delta);
	float interaction = 0.0f;

	if (perception.hasCarAhead)
	{
		float s = std::max(
			perception.distanceToCarAhead,
			0.1f
		);

		float dv = perception.relativeSpeed;

		float desiredGap =
			2.5f +
			s0 +
			v * T +
			(v * dv) /
			(2.0f * std::sqrt(maxAccel * maxDecel + 0.001f));

		float ratio = desiredGap / s;
		interaction = ratio * ratio;
	}

	float accel =
		(maxAccel * accelFactor)
		*
		(1.0f - freeRoad - interaction);

	accel = std::clamp(
		accel,
		-maxDecel,
		maxAccel
	);

	return accel;
}

void IDMLongitudinalModel::setDriverFactors(
	float gapFactor,
	float accelFactor)
{
	this->gapFactor =
		gapFactor;

	this->accelFactor =
		accelFactor;
}