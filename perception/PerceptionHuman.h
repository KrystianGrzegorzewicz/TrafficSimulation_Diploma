#pragma once

#include "perception/IPerception.h"

class PerceptionHuman : public IPerception
{
public:
	void update(
		const CarState& self,
		const WorldState& world,
		PerceptionState& outState
	) override;

private:
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