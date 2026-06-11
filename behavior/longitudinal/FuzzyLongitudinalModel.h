#pragma once

#include "behavior/longitudinal/rondo2_fis.h"
#include "behavior/longitudinal/headtohead_fis.h"

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