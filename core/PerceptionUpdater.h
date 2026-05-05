#pragma once
#include "core/Perception.h"
#include <vector>

class PerceptionUpdater
{
public:
    static void update(
        Perception& perception,
        const std::vector<CarState>& others
    );
};