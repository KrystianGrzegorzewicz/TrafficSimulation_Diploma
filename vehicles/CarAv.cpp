#include "vehicles/CarAV.h"

CarAV::CarAV(
	float initialSpeed,
	Travel travel,
	std::unique_ptr<IBehavior> beh,
	std::unique_ptr<IPerception> perc)
	: Car(initialSpeed, std::move(travel)),
	behavior(std::move(beh)),
	perception(std::move(perc))
{
	this->color[0] = 180;
	this->color[1] = 180;
	this->color[2] = 10;

	// AV tuning
	maxSpeed = 23.0f;
	maxAccel = 5.0f;
	maxDecel = 7.0f;

	kp = 8.0f;
	kd = 2.0f;

	steering = SteeringModel(kp, kd);
}

void CarAV::update(
	float dt,
	const WorldState& world)
{
	executeUpdate(
		dt,
		world,
		*behavior,
		*perception,
		perceptionState);
}