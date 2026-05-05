#include "core/Block.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Block::Block(float x0, float y0, float xn, float yn, float onDur, float offDur) 
    : x0y0(x0, y0), xnyn(xn, yn), onDuration(onDur), offDuration(offDur), 
      elapsedTime(0.0f), isCurrentlyOn(false) 
{
    Block::color[0] = 255;
    Block::color[1] = 255;
    Block::color[2] = 255;
}

void Block::update(float dt)
{
    elapsedTime += dt;
    float cycleDuration = onDuration + offDuration;
    
    // Wrap elapsed time to current cycle
    if (elapsedTime >= cycleDuration)
    {
        elapsedTime = std::fmod(elapsedTime, cycleDuration);
    }
    
    // Determine if currently on or off
    isCurrentlyOn = (elapsedTime < onDuration);
}

void Block::getVisualization() {
    std::cout << "Block: (" << x0y0.x << ", " << x0y0.y << ") to (" << xnyn.x << ", " << xnyn.y << ") with color ("
        << color[0] << ", " << color[1] << ", " << color[2] << ")"
        << " - State: " << (isCurrentlyOn ? "ON" : "OFF")
        << " (" << elapsedTime << "s)" << std::endl;
}

Vec2 Block::getTopLeft() const {
    return x0y0;
}

Vec2 Block::getBottomRight() const {
    return xnyn;
}

bool Block::isActive() const
{
    return isCurrentlyOn;
}

float Block::getProgress() const
{
    float cycleDuration = onDuration + offDuration;
    return elapsedTime / cycleDuration;
}

Vec2 Block::getCenter() const
{
    return Vec2(
        (x0y0.x + xnyn.x) * 0.5f,
        (x0y0.y + xnyn.y) * 0.5f
    );
}

bool Block::containsPoint(const Vec2& point) const
{
    float minX = std::min(x0y0.x, xnyn.x);
    float maxX = std::max(x0y0.x, xnyn.x);
    float minY = std::min(x0y0.y, xnyn.y);
    float maxY = std::max(x0y0.y, xnyn.y);
    
    return point.x >= minX && point.x <= maxX &&
           point.y >= minY && point.y <= maxY;
}

float Block::getDistanceToPoint(const Vec2& point) const
{
    float minX = std::min(x0y0.x, xnyn.x);
    float maxX = std::max(x0y0.x, xnyn.x);
    float minY = std::min(x0y0.y, xnyn.y);
    float maxY = std::max(x0y0.y, xnyn.y);
    
    float dx = 0.0f;
    float dy = 0.0f;
    
    if (point.x < minX)
        dx = minX - point.x;
    else if (point.x > maxX)
        dx = point.x - maxX;
    
    if (point.y < minY)
        dy = minY - point.y;
    else if (point.y > maxY)
        dy = point.y - maxY;
    
    return std::sqrt(dx * dx + dy * dy);
}
