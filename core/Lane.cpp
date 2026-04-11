#include "Lane.h"
#include <cmath>

Lane::Lane(Vec2 s, Vec2 e) : start(s), end(e) {
    float dx = e.x - s.x;
    float dy = e.y - s.y;
    length = sqrt(dx * dx + dy * dy);
}

Vec2 Lane::getPoint(float t) const {
    return start + (end - start) * t;
}

Vec2 Lane::getDirection() const {
    Vec2 dir = end - start;
    float len = sqrt(dir.x * dir.x + dir.y * dir.y);
    return Vec2(dir.x / len, dir.y / len);
}

float Lane::getLength() const {
    return length;
}