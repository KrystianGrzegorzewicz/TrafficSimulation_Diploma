#include "vehicles/CarHuman.h"

CarHuman::CarHuman(
    float initialSpeed,
    Travel travel,
    std::unique_ptr<IBehavior> beh,
    std::unique_ptr<IPerception> perc
)
    : Car(initialSpeed, std::move(travel)),
    behavior(std::move(beh)),
    perception(std::move(perc))
{
    color[0] = 20;
    color[1] = 200;
    color[2] = 50;
}

void CarHuman::update(float dt, const WorldState& world)
{
    if (!isPathValid() || isFinishedInternal()) return;

    updateClosestT();
    if (advanceSegmentIfNeeded()) return;

    perception->update(getState(), world, perceptionState);

    MotionCommand cmd = behavior->compute(
        travel,
        segment,
        t,
        getState(),
        maxSpeed,
        maxAccel,
        maxDecel,
        lookaheadBase,
        lookaheadSpeedFactor,
        perceptionState
    );

    Vec2 latAccel = steering.computeLateralAcceleration(
        position,
        velocity,
        cmd.targetPoint
    );

    Vec2 forwardDir = velocity.length() > 0.1f
        ? velocity.normalized()
        : Vec2(1.f, 0.f);

    Vec2 desiredAccel = forwardDir * cmd.longitudinalAcceleration;

    if (cmd.emergencyBrake)
    {
        desiredAccel = forwardDir * (-maxDecel);
    }

    integrate(desiredAccel + latAccel, dt);
}