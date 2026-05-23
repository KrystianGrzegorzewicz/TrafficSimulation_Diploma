#pragma once

#include "behavior/longitudinal/ILongitudinalModel.h"

// Placeholder for future fuzzy logic controller.
//
// A GitHub fuzzy library can later be plugged here
// without modifying BehaviorHuman or CarHuman.
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