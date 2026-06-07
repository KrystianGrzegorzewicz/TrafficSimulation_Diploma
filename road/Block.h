#pragma once

#include "core/Vec2.h"

class Block {
private:
	Vec2 x0y0, xnyn;
	int color[3];
	float onDuration;
	float offDuration;
	float elapsedTime;
	bool isCurrentlyOn;

public:
	Block(float x0, float y0, float xn, float yn, float onDur = 0.0f, float offDur = 1.0f, float initTime = 0.0f);
	~Block() = default;

	void update(float dt);

	// Original methods
	void getVisualization();
	int getColor(int index);
	Vec2 getTopLeft() const;
	Vec2 getBottomRight() const;

	// Hazard-related methods
	bool isActive() const;
	float getProgress() const;  // Returns 0.0f to 1.0f of current cycle
	Vec2 getCenter() const;
	bool containsPoint(const Vec2& point) const;
	float getDistanceToPoint(const Vec2& point) const;
};