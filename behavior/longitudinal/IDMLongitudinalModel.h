#pragma once

#include "behavior/longitudinal/ILongitudinalModel.h"

class IDMLongitudinalModel : public ILongitudinalModel
{
public:
	float computeAcceleration(
		const CarState& self,
		const PerceptionState& perception,
		float desiredSpeed,
		float maxAccel,
		float maxDecel
	) override;

	void setDriverFactors(
		float gapFactor,
		float accelFactor);
private:
	float gapFactor = 1.0f;
	float accelFactor = 1.0f;
};