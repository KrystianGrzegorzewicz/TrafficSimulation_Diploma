#pragma once

#include "behavior/longitudinal/ILongitudinalModel.h"

class FuzzyLongitudinalModel : public ILongitudinalModel
{
public:
	float computeAcceleration(
		const CarState& self,
		const PerceptionState& perception,
		float desiredSpeed,
		float maxAccel,
		float maxDecel
	) override;
};