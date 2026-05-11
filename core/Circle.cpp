#include "core/Circle.h"

Circle::Circle(float cx, float cy, float radius, bool filled)
    : center(cx, cy), radius(radius), filled(filled) {
    // Default light grayish color
    color[0] = 200;  // R
    color[1] = 200;  // G
    color[2] = 200;  // B
}

Vec2 Circle::getCenter() const {
    return center;
}

float Circle::getRadius() const {
    return radius;
}

bool Circle::isFilled() const {
    return filled;
}

int Circle::getColor(int index) const {
    if (index >= 0 && index < 3)
        return color[index];
    return 0;
}

void Circle::setColor(int r, int g, int b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}
