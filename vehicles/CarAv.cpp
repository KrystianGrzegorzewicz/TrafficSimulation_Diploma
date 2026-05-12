#include "vehicles/CarAV.h"

/*CarAV::CarAV(
	float                        initialSpeed,
	Travel                       travel,
	std::unique_ptr<IBehavior>   behavior,
	std::unique_ptr<IPerception> perception
)
	: Car(initialSpeed, std::move(travel)),
	behavior(std::move(behavior)),
	perception(std::move(perception))
{
	this->color[0] = 180;
	this->color[1] = 180;
	this->color[2] = 10;
}

void CarAV::update(float dt, const WorldState& world)
{
	if (!isPathValid() || isFinishedInternal()) return;

	updateClosestT();
	if (advanceSegmentIfNeeded()) return;

	// 1. Perception — AV stub fills nothing yet
	perception->update(getState(), world, perceptionState);

	// 2. Behavior — AV stub returns zero acceleration
	BehaviorOutput behOut = behavior->compute(
		travel, segment, t,
		velocity.length(), maxSpeed,
		maxAccel, maxDecel,
		lookaheadBase, lookaheadSpeedFactor,
		perceptionState
	);

	// 3. Steering — shared physics, same as CarHuman
	Vec2 latAccel = steering.computeLateralAcceleration(
		travel, segment, t,
		position, velocity,
		lookaheadBase, lookaheadSpeedFactor,
		behOut
	);

	// 4. Integrate
	integrate(behOut.acceleration + latAccel, dt);
}*/