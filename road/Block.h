#pragma once

#include "core/Vec2.h"

#include <vector>

enum class BlockType
{
	Visual,
	Stop,
	PerceptionMask
};

class Block
{
private:
	Vec2 x0y0;
	Vec2 xnyn;

	int color[3];

	float onDuration;
	float offDuration;
	float elapsedTime;

	bool isCurrentlyOn;

	BlockType type;

	std::vector<int>* maskedTravelIds;

public:
	Block();
	Block(
		float x0,
		float y0,
		float xn,
		float yn,
		float onDur = 0.0f,
		float offDur = 1.0f,
		float initTime = 0.0f,
		BlockType type = BlockType::Stop,
		std::vector<int> maskedTravels = {}
	);

	~Block() = default;

	void update(float dt);

	void getVisualization();

	int getColor(int index);

	Vec2 getTopLeft() const;
	Vec2 getBottomRight() const;

	bool isActive() const;

	float getProgress() const;

	Vec2 getCenter() const;

	bool containsPoint(const Vec2& point) const;

	float getDistanceToPoint(const Vec2& point) const;

	BlockType getType() const;

	bool masksTravel(int travelId) const;
};