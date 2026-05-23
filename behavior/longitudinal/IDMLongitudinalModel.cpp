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
	constexpr float s0 = 3.0f;
	constexpr float T = 1.2f;

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
			s0 +
			v * T +
			(v * dv) /
			(2.0f * std::sqrt(maxAccel * maxDecel + 0.001f));

		float ratio = desiredGap / s;
		interaction = ratio * ratio;
	}

	float accel =
		maxAccel *
		(1.0f - freeRoad - interaction);

	return std::clamp(
		accel,
		-maxDecel,
		maxAccel
	);
}