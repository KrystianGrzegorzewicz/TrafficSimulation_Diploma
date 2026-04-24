#pragma once
#include "core/CarState.h"
#include <vector>

struct Perception
{
    CarState self;

    bool hasCarAhead = false;
    CarState carAhead;

    float distanceToCarAhead = 0.0f;
    float relativeSpeed = 0.0f;

    float relativeAcceleration = 0.0f;
};

void updatePerception(Perception& p, const std::vector<CarState>& others);