#pragma once
#include "road/Travel.h"
#include "vehicles/CarState.h"
#include "physics/SteeringModel.h"
#include "core/Vec2.h"
#include "perception/IPerception.h"
#include "behavior/IBehavior.h"

// Base vehicle.  Owns:
//   - physical state (position, velocity, acceleration)
//   - path tracking (segment, t)
//   - SteeringModel (shared physics, not a strategy)
//   - integrate() — Euler integration, same for all vehicle types
//
// Does NOT own IBehavior or IPerception — subclasses inject these.
// Simulation holds std::unique_ptr<Car>, calling the virtual update().
struct WorldState;
//class IPerception;

class Car
{
public:
	Car(float initialSpeed, Travel travel);
	virtual ~Car();

	// Called each tick.  Subclasses implement their own decision cycle
	// but MUST call integrate() with the desired acceleration.
	virtual void update(float dt, const WorldState& world) = 0;

	// --- Accessors (unchanged API) ---
	int	getId()               const;
	int	getTravelId()         const;
	Vec2 getPosition()         const;
	Vec2 getVelocityVector()   const;
	Vec2 getAccelerationVector() const;
	bool isFinished()	const;
	int* getColor()	const;
	Vec2 filteredTargetPoint;
	bool hasFilteredTarget = false;

	CarState getState()	const;  // convenience for WorldState assembly

protected:
	// Shared path helpers
	bool isPathValid()          const;
	bool isFinishedInternal();
	void updateClosestT();
	bool advanceSegmentIfNeeded();

	// Shared physics — called by every subclass
	void integrate(const Vec2& desiredAcceleration, float dt);
	void executeUpdate(
		float dt,
		const WorldState& world,
		IBehavior& behavior,
		IPerception& perception,
		PerceptionState& perceptionState
	);

	// --- State ---
	static int nextId;
	int   id;
	int   travelId;
	Travel travel;
	int   segment = 0;
	float t = 0.0f;
	bool  finished = false;
	int color[3];

	Vec2  position;
	Vec2  velocity;
	Vec2  acceleration;
	float speed = 0.0f;

	// --- Tuning (subclasses may override in constructor) ---
	float maxAccel = 4.0f;
	float maxDecel = 6.0f;
	float maxSpeed = 20.0f;
	float lookaheadBase = 0.1f;
	float lookaheadSpeedFactor = 0.03f;
	float kp = 5.0f;
	float kd = 3.0f;

	SteeringModel steering{ kp, kd };
};