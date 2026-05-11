#pragma once
#include "perception/IPerception.h"

// Human-driver perception model.
// Implements:
//   - Dynamic FOV (speed- and lateral-acceleration-driven cone narrowing)
//   - TTC-scored car-ahead detection
//   - Block hazard detection (replaces old BlockPerception class)
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
        const std::vector<CarState>& others,
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
    static constexpr float kMaxViewDistMax = 80.0f;
    static constexpr float kFovAngleNarrow = 50.0f;  // degrees, high speed
    static constexpr float kFovAngleWide = 120.0f; // degrees, low speed
    static constexpr float kBlockMaxView = 70.0f;
    static constexpr float kBlockThreatDist = 50.0f;
};