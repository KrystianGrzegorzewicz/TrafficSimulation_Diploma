#pragma once
#include "vehicles/Car.h"
#include "behavior/IBehavior.h"
#include "perception/IPerception.h"
#include <memory>

// Skeleton — structure mirrors CarHuman, logic left for future sprint.
class CarAV : public Car
{
public:
	CarAV(
		float initialSpeed,
		Travel travel,
		std::unique_ptr<IBehavior>   behavior,
		std::unique_ptr<IPerception> perception
	);

	void update(float dt, const WorldState& world) override;

private:
	std::unique_ptr<IBehavior>   behavior;
	std::unique_ptr<IPerception> perception;
	PerceptionState perceptionState;
};