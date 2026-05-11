#pragma once
#include "perception/PerceptionState.h"
#include "simulation/WorldState.h"  // fwd included via header
#include "vehicles/CarState.h"

// Strategy interface — every vehicle type plugs in its own implementation.
class IPerception
{
public:
	virtual ~IPerception() = default;

	// Update perceptual state for one vehicle.
	// self    — the owning vehicle's current state
	// world   — full simulation context (other vehicles, road blocks, etc.)
	// outState — filled in by the implementation
	virtual void update(
		const CarState& self,
		const WorldState& world,
		PerceptionState& outState
	) = 0;
};