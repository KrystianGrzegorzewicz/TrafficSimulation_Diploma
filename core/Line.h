#pragma once
#include "core/Vec2.h"

class Line {
private:
    Vec2 start;
    Vec2 end;
    int color[3];
    float thickness;
    
public:
    Line(float x1, float y1, float x2, float y2, float thickness = 1.0f);
    ~Line() = default;
    
    Vec2 getStart() const;
    Vec2 getEnd() const;
    float getThickness() const;
    int getColor(int index) const;
    
    void setColor(int r, int g, int b);
};
