#include "behavior/longitudinal/FuzzyLongitudinalModel.h"

#include <cmath>
#include <algorithm>

float FuzzyLongitudinalModel::computeAcceleration(
	const CarState& self,
	const PerceptionState& perception,
	float desiredSpeed,
	float maxAccel,
	float maxDecel
)
{
	float accel = 0.0f;
	float inputs1[2] = { self.velocity.length(), desiredSpeed };
	float output1[1];

	float inputs2[2] = { perception.distanceToCarAhead > 100.0f ? 100.0f : perception.distanceToCarAhead, perception.relativeSpeed };
	float output2[1];

	pathPlanning_run(
		inputs1,
		output1
	);
	carFollow_run(
		inputs2,
		output2
	);
	accel = std::min(output1[0], output2[0]);

	return accel;
}