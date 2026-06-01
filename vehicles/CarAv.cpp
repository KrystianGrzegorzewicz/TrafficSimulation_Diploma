#include "vehicles/CarAv.h"
#include "perception/IPerception.h"

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
	maxSpeed = 18.0f;
	maxAccel = 3.7f;
	maxDecel = 5.5f;

	/*kp = 8.0f;
	kd = 2.0f;*/

	steering = SteeringModel(kp, kd);
}
CarAV::~CarAV() = default;

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