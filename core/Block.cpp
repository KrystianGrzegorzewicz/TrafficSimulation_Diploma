#include "core/Block.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Block::Block(float x0, float y0, float xn, float yn, float onDur, float offDur)
    : x0y0(x0, y0), xnyn(xn, yn), onDuration(onDur), offDuration(offDur),
      elapsedTime(0.0f), isCurrentlyOn(true)
{
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
}

void Block::update(float dt)
{
    elapsedTime += dt;
    float cycleDuration = onDuration + offDuration;
    
    // Wrap around cycle
    if (elapsedTime >= cycleDuration)
    {
        elapsedTime = std::fmod(elapsedTime, cycleDuration);
    }
    
    // Determine current state
    isCurrentlyOn = (elapsedTime < onDuration);
    
    // Update color based on state
    if (isCurrentlyOn)
    {
        color[0] = 200;  // Bright red when active
        color[1] = 0;
        color[2] = 0;
    }
    else
    {
        color[0] = 80;   // Dark red when inactive
        color[1] = 20;
        color[2] = 20;
    }
}

void Block::getVisualization()
{
    std::string state = isCurrentlyOn ? "ON" : "OFF";
    std::cout << "Block: (" << x0y0.x << ", " << x0y0.y << ") to ("
              << xnyn.x << ", " << xnyn.y << ") [" << state << "] with color ("
              << color[0] << ", " << color[1] << ", " << color[2] << ")"
              << std::endl;
}

int Block::getColor(int index)
{
    if (index >= 0 && index < 3)
        return color[index];
    return 0;
}

Vec2 Block::getTopLeft() const
{
    return x0y0;
}

Vec2 Block::getBottomRight() const
{
    return xnyn;
}

bool Block::isActive() const
{
    return isCurrentlyOn;
}

float Block::getProgress() const
{
    float cycleDuration = onDuration + offDuration;
    if (cycleDuration < 0.001f)
        return 0.0f;
    return std::fmod(elapsedTime, cycleDuration) / cycleDuration;
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
    
    // Clamp point to block bounds
    float closestX = std::clamp(point.x, minX, maxX);
    float closestY = std::clamp(point.y, minY, maxY);
    
    // Calculate distance to closest point on block
    float dx = point.x - closestX;
    float dy = point.y - closestY;
    
    return std::sqrt(dx * dx + dy * dy);
}