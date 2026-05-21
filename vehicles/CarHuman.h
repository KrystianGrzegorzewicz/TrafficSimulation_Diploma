#pragma once
#include "vehicles/Car.h"

#include <memory>

class CarHuman : public Car
{
public:
	CarHuman(
		float initialSpeed,
		Travel travel,
		std::unique_ptr<IBehavior>   behavior,
		std::unique_ptr<IPerception> perception//,
		//int color[3]
	);
	~CarHuman() override;

	void update(float dt, const WorldState& world) override;

private:
	std::unique_ptr<IBehavior>   behavior;
	std::unique_ptr<IPerception> perception;
	PerceptionState perceptionState;  // reused each tick
};