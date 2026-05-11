#include "core/Line.h"

Line::Line(float x1, float y1, float x2, float y2, float thickness)
    : start(x1, y1), end(x2, y2), thickness(thickness) {
    // Default light grayish color
    color[0] = 200;  // R
    color[1] = 200;  // G
    color[2] = 200;  // B
}

Vec2 Line::getStart() const {
    return start;
}

Vec2 Line::getEnd() const {
    return end;
}

float Line::getThickness() const {
    return thickness;
}

int Line::getColor(int index) const {
    if (index >= 0 && index < 3)
        return color[index];
    return 0;
}

void Line::setColor(int r, int g, int b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}
