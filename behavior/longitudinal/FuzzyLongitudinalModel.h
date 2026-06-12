#pragma once

#include "behavior/longitudinal/carFollow_fis.h"
#include "behavior/longitudinal/pathPlanning_fis.h"

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
private:
};