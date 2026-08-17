#pragma once

#include "vehicles/Car.h"

#include <memory>

class CarAV : public Car
{
public:
	CarAV(
		float initialSpeed,
		Travel travel,
		std::unique_ptr<IBehavior> behavior,
		std::unique_ptr<IPerception> perception
	);
	~CarAV() override;

	void update(float dt, const WorldState& world) override;
	std::string getTypeName() const override { return "av"; }
	bool isAutonomous() const override { return true; }

private:
	std::unique_ptr<IBehavior> behavior;
	std::unique_ptr<IPerception> perception;
	PerceptionState perceptionState;
};