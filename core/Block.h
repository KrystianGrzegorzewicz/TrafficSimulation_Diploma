#pragma once
#include "core/Vec2.h"

class Block {
private:
    Vec2 x0y0, xnyn;
    int color[3];
    
    // Timing properties
    float onDuration;      // Duration block is active (seconds)
    float offDuration;     // Duration block is inactive (seconds)
    float elapsedTime;     // Current elapsed time in current cycle
    bool isCurrentlyOn;    // Current state
    
public:
    Block(float x0, float y0, float xn, float yn, float onDur = 5.0f, float offDur = 5.0f);
    
    void update(float dt);
    void getVisualization();
    int getColor(int index);
    Vec2 getTopLeft() const;
    Vec2 getBottomRight() const;
    
    // Hazard-related methods
    bool isActive() const;
    float getProgress() const;  // Returns 0.0f to 1.0f of current cycle
    Vec2 getCenter() const;
    
    // Collision detection
    bool containsPoint(const Vec2& point) const;
    float getDistanceToPoint(const Vec2& point) const;
};
