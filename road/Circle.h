#pragma once

#include "core/Vec2.h"

class Circle {
private:
	Vec2 center;
	float radius;
	int color[3];
	bool filled;

public:
	Circle(float cx, float cy, float radius, bool filled = false);
	~Circle() = default;

	Vec2 getCenter() const;
	float getRadius() const;
	bool isFilled() const;
	int getColor(int index) const;
	void setColor(int r, int g, int b);
};
