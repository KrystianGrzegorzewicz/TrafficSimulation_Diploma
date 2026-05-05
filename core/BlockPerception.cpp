#include "core/BlockPerception.h"
#include <algorithm>
#include <cmath>
#include <limits>

void BlockPerception::updateBlockPerception(
    const std::vector<Block>& blocks,
    const CarState& self,
    BlockHazard& outHazard
)
{
    outHazard.blockIndex = -1;
    outHazard.distance = 999999.0f;
    outHazard.isActive = false;
    outHazard.threat = 0.0f;

    if (blocks.empty())
        return;

    // Determine forward direction
    Vec2 forward = (self.velocity.length() > 0.5f)
        ? self.velocity.normalized()
        : Vec2(1, 0);

    float bestThreat = 0.0f;
    int bestBlockIndex = -1;
    float bestDistance = 999999.0f;

    for (size_t i = 0; i < blocks.size(); ++i)
    {
        Vec2 blockCenter = blocks[i].getCenter();
        Vec2 toBlock = blockCenter - self.position;
        float distance = toBlock.length();

        // Only consider blocks within view distance
        if (distance > MAX_VIEW_DISTANCE)
            continue;

        // Only consider blocks ahead of the car
        if (distance > 0.1f)
        {
            Vec2 dirToBlock = toBlock / distance;
            float dotProduct = forward.dot(dirToBlock);
            if (dotProduct < 0.3f)  // Behind or far to the side
                continue;
        }

        // Calculate threat level
        float threat = 0.0f;
        if (blocks[i].isActive())
        {
            // Threat increases as we get closer
            threat = std::max(0.0f, 1.0f - (distance / THREAT_DISTANCE));
            threat = std::clamp(threat, 0.0f, 1.0f);
        }

        // Update best hazard if this block is closer and active, or has higher threat
        if ((blocks[i].isActive() && threat > bestThreat) ||
            (blocks[i].isActive() && distance < bestDistance && bestBlockIndex >= 0 && blocks[bestBlockIndex].isActive()))
        {
            bestThreat = threat;
            bestBlockIndex = (int)i;
            bestDistance = distance;
        }
        else if (bestBlockIndex < 0 && distance < bestDistance)
        {
            // If no active block found yet, track closest block
            bestBlockIndex = (int)i;
            bestDistance = distance;
        }
    }

    // Update output
    if (bestBlockIndex >= 0)
    {
        outHazard.blockIndex = bestBlockIndex;
        outHazard.distance = bestDistance;
        outHazard.isActive = blocks[bestBlockIndex].isActive();
        outHazard.threat = blocks[bestBlockIndex].isActive() ? bestThreat : 0.0f;
    }
}