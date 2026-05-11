#include "vehicles/CarHuman.h"

CarHuman::CarHuman(
	float                        initialSpeed,
	Travel                       travel,
	std::unique_ptr<IBehavior>   beh,
	std::unique_ptr<IPerception> perc
)
	: Car(initialSpeed, std::move(travel)),
	behavior(std::move(beh)),
	perception(std::move(perc))
{
	this->color[0] = 20;
	this->color[1] = 200;
	this->color[2] = 50;
}

void CarHuman::update(float dt, const WorldState& world)
{
	if (!isPathValid() || isFinishedInternal()) return;

	updateClosestT();
	if (advanceSegmentIfNeeded()) return;

	// 1. Perception — fills perceptionState from WorldState snapshot
	perception->update(getState(), world, perceptionState);

	// 2. Behavior — longitudinal acceleration + lookahead target point
	BehaviorOutput behOut = behavior->compute(
		travel, segment, t,
		velocity.length(), maxSpeed,
		maxAccel, maxDecel,
		lookaheadBase, lookaheadSpeedFactor,
		perceptionState
	);

	// 3. Steering — lateral acceleration (shared physics, not a strategy)
	Vec2 latAccel = steering.computeLateralAcceleration(
		travel, segment, t,
		position, velocity,
		lookaheadBase, lookaheadSpeedFactor,
		behOut
	);

	// 4. Integrate combined acceleration
	integrate(behOut.acceleration + latAccel, dt);
}