#pragma once

#include "perception/IPerception.h"
#include "vehicles/DriverPersonality.h"

class PerceptionHuman : public IPerception
{
public:
	explicit PerceptionHuman(const DriverPersonality& p) : personality(p) {}

	void update(const CarState& self, const WorldState& world, PerceptionState& out) override;

private:
	DriverPersonality personality;

	void updateCarAhead(
		const CarState& self,
		const WorldState& world,
		PerceptionState& out
	);

	void updateBlockHazard(
		const CarState& self,
		const WorldState& world,
		PerceptionState& out
	);

	void updateConflictPoints(
		const CarState& self,
		const WorldState& world,
		PerceptionState& out
	);

	struct FOVResult { float fovDot; float maxViewDistance; };
	FOVResult calculateFOV(const CarState& self) const;

	// --- tuning ---
	static constexpr float kLaneWidth = 3.0f;
	static constexpr float kMaxViewDistMin = 30.0f;
	static constexpr float kMaxViewDistMax = 100.0f;
	static constexpr float kFovAngleNarrow = 40.0f;  // degrees, high speed
	static constexpr float kFovAngleWide = 160.0f; // degrees, low speed
	static constexpr float kBlockMaxView = 100.0f;
	static constexpr float kBlockThreatDist = 150.0f;
};