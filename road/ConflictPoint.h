#pragma once

#include "core/Vec2.h"

#include <vector>

struct ConflictPoint
{
	Vec2 position;

	std::vector<int> priorityTravels;
	std::vector<int> yieldTravels;

	float radius = 4.f;

	bool affects(int travelId) const
	{
		for (int x : priorityTravels)
			if (x == travelId)
				return true;

		for (int x : yieldTravels)
			if (x == travelId)
				return true;

		return false;
	}

	bool hasPriority(int travelId) const
	{
		for (int x : priorityTravels)
			if (x == travelId)
				return true;

		return false;
	}

	bool mustYield(int travelId) const
	{
		for (int x : yieldTravels)
			if (x == travelId)
				return true;

		return false;
	}
};