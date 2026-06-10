#pragma once

#include "road/Travel.h"
#include "vehicles/CarState.h"
#include "physics/SteeringModel.h"
#include "core/Vec2.h"
#include "perception/IPerception.h"
#include "behavior/IBehavior.h"
#include "vehicles/DriverPersonality.h"

struct WorldState;

struct Anfis {
	float desiredSpeed;
	float relDist;
	float relSpeed;
	float relAccel;
	float output;
};

class Car
{
public:
	Car(float initialSpeed, Travel travel);
	virtual ~Car();

	virtual void update(float dt, const WorldState& world) = 0;
	int	getId() const;
	int	getTravelId() const;
	Vec2 getPosition() const;
	Vec2 getVelocityVector() const;
	float getSpeed() const;
	float getDesiredSpeed() const;
	Vec2 getAccelerationVector() const;
	bool isFinished() const;
	int* getColor()	const;
	Vec2 filteredTargetPoint;
	bool hasFilteredTarget = false;
	Anfis output;

	CarState getState()	const;
	Anfis getAnfisOutput() const
	{
		return output;
	}
	virtual const DriverPersonality* getPersonality() const
	{
		return nullptr;
	}

protected:
	bool isPathValid()          const;
	bool isFinishedInternal();
	void updateClosestT();
	bool advanceSegmentIfNeeded();
	Vec2 getPathForward() const;
	void integrate(const Vec2& desiredAcceleration, float dt);
	void executeUpdate(
		float dt,
		const WorldState& world,
		IBehavior& behavior,
		IPerception& perception,
		PerceptionState& perceptionState
	);

	static int nextId;
	int id;
	int travelId;
	Travel travel;
	int segment = 0;
	float t = 0.0f;
	bool finished = false;
	int color[3] = { 255, 255, 255 };
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 forward;
	float speed = 0.0f;

	//Tuning
	float maxAccel = 4.0f;
	float maxDecel = 6.0f;
	float maxSpeed = 20.0f;
	float lookaheadBase = 0.1f;
	float lookaheadSpeedFactor = 0.04f;
	float kp = 8.0f;
	float kd = 6.0f;

	SteeringModel steering{ kp, kd };
};