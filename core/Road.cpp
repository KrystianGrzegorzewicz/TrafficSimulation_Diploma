#include "Road.h"

void Road::addLane(const Lane& lane) {
    lanes.push_back(lane);
}

const std::vector<Lane>& Road::getLanes() const {
    return lanes;
}