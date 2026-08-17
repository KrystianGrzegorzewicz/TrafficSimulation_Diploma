#pragma once

#include "vehicles/Car.h"
#include "vehicles/DriverPersonality.h"

#include <memory>

class CarHuman : public Car
{
public:
	CarHuman(
		float initialSpeed,
		Travel travel,
		DriverPersonality personality,
		std::unique_ptr<IBehavior> behavior,
		std::unique_ptr<IPerception> perception
	);
	~CarHuman() override;

	void update(float dt, const WorldState& world) override;
	std::string getTypeName() const override { return "human"; }
	const DriverPersonality* getPersonality() const override { return &personality; }

private:
	std::unique_ptr<IBehavior> behavior;
	std::unique_ptr<IPerception> perception;
	PerceptionState perceptionState;
	DriverPersonality personality;
};