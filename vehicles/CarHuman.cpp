#include "vehicles/CarHuman.h"
#include "perception/IPerception.h"

CarHuman::CarHuman(
	float initialSpeed,
	Travel travel,
	std::unique_ptr<IBehavior> beh,
	std::unique_ptr<IPerception> perc)
	: Car(initialSpeed, std::move(travel)),
	behavior(std::move(beh)),
	perception(std::move(perc))
{
	color[0] = 20;
	color[1] = 200;
	color[2] = 50;

	// Human tuning
	maxSpeed = 18.0f;
	maxAccel = 3.5f;
	maxDecel = 5.5f;

	/*kp = 6.5f;
	kd = 2.5f;*/

	steering = SteeringModel(kp, kd);
}
CarHuman::~CarHuman() = default;

void CarHuman::update(
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