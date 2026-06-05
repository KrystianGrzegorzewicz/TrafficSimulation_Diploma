#include "road/Block.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Block::Block(float x0, float y0, float xn, float yn,
	float onDur, float offDur, float initTime)
	: x0y0(x0, y0), xnyn(xn, yn)
	, onDuration(onDur), offDuration(offDur)
	, elapsedTime(initTime), isCurrentlyOn(true)
{
	color[0] = 255;
	color[1] = 0;
	color[2] = 0;
}

void Block::update(float dt)
{
	elapsedTime += dt;
	float cycle = onDuration + offDuration;

	if (elapsedTime >= cycle)
		elapsedTime = std::fmod(elapsedTime, cycle);

	isCurrentlyOn = (elapsedTime < onDuration);

	if (isCurrentlyOn)
	{
		color[0] = 200; color[1] = 0;  color[2] = 0;
	}
	else
	{
		color[0] = 80;  color[1] = 20; color[2] = 20;
	}
}

void Block::getVisualization()
{
	std::cout << "Block: (" << x0y0.x << ", " << x0y0.y << ") to ("
		<< xnyn.x << ", " << xnyn.y << ") ["
		<< (isCurrentlyOn ? "ON" : "OFF") << "]\n";
}

int  Block::getColor(int index)
{
	if (index >= 0 && index < 3) return color[index];
	return 0;
}

Vec2 Block::getTopLeft()     const { return x0y0; }
Vec2 Block::getBottomRight() const { return xnyn; }
bool Block::isActive()       const { return isCurrentlyOn; }

float Block::getProgress() const
{
	float cycle = onDuration + offDuration;
	if (cycle < 0.001f) return 0.f;
	return std::fmod(elapsedTime, cycle) / cycle;
}

Vec2 Block::getCenter() const
{
	return Vec2((x0y0.x + xnyn.x) * 0.5f,
		(x0y0.y + xnyn.y) * 0.5f);
}

bool Block::containsPoint(const Vec2& point) const
{
	float minX = std::min(x0y0.x, xnyn.x);
	float maxX = std::max(x0y0.x, xnyn.x);
	float minY = std::min(x0y0.y, xnyn.y);
	float maxY = std::max(x0y0.y, xnyn.y);
	return point.x >= minX && point.x <= maxX
		&& point.y >= minY && point.y <= maxY;
}

float Block::getDistanceToPoint(const Vec2& point) const
{
	float minX = std::min(x0y0.x, xnyn.x);
	float maxX = std::max(x0y0.x, xnyn.x);
	float minY = std::min(x0y0.y, xnyn.y);
	float maxY = std::max(x0y0.y, xnyn.y);

	float cx = std::clamp(point.x, minX, maxX);
	float cy = std::clamp(point.y, minY, maxY);
	float dx = point.x - cx;
	float dy = point.y - cy;
	return std::sqrt(dx * dx + dy * dy);
}