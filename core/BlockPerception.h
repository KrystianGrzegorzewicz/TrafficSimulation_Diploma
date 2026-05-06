#pragma once
#include "core/Vec2.h"
#include "core/Block.h"
#include "core/CarState.h"
#include <vector>

struct BlockHazard
{
    int blockIndex = -1;          // -1 if no block detected
    float distance = 999999.0f;   // Distance to block
    bool isActive = false;        // Is block currently on?
    float threat = 0.0f;          // 0-1 threat level based on distance and speed
};

class BlockPerception
{
public:
    static void updateBlockPerception(
        const std::vector<Block>& blocks,
        const CarState& self,
        BlockHazard& outHazard
    );

private:
    static constexpr float MAX_VIEW_DISTANCE = 70.0f;
    static constexpr float THREAT_DISTANCE = 50.0f;  // Distance at which threat = 1.0f
};