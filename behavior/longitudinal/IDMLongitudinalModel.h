#pragma once

#include "behavior/longitudinal/ILongitudinalModel.h"

// Intelligent Driver Model (IDM)
//
// Literature:
// Treiber, Hennecke & Helbing
// "Congested traffic states in empirical observations and microscopic simulations"
// and
// Treiber & Kesting - Traffic Flow Dynamics
//
// IDM equation:
//
// a = a_max * [1 - (v/v0)^delta - (s*/s)^2]
//
// where:
// v  = ego speed
// v0 = desired speed
// s  = current gap
// s* = desired dynamic gap
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
};