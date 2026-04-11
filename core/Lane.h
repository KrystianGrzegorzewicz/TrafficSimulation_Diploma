#pragma once
#pragma once
#include "Vec2.h"

class Lane {
private:
    Vec2 start;
    Vec2 end;
    float length;

public:
    Lane(Vec2 s, Vec2 e);

    Vec2 getPoint(float t) const;   // pozycja 0..1 na pasie
    Vec2 getDirection() const;      // kierunek jazdy
    float getLength() const;
};