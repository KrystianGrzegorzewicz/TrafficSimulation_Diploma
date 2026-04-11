#pragma once
#include <vector>
#include "Lane.h"

class Road {
private:
    std::vector<Lane> lanes;

public:
    Road() = default;

    void addLane(const Lane& lane);
    const std::vector<Lane>& getLanes() const;
};