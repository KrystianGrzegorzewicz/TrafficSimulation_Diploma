#pragma once
#include "perception/IPerception.h"

// Skeleton — no logic yet.
// Will receive sensor fusion, LIDAR model, etc.
class PerceptionAV : public IPerception
{
public:
    void update(
        const CarState& self,
        const WorldState& world,
        PerceptionState& outState
    ) override
    {
        // TODO: AV sensor model
        (void)self; (void)world; (void)outState;
    }
};