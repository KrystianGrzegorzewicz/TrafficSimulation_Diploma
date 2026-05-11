#include "vehicles/Car.h"
#include <algorithm>
#include <cmath>

int Car::nextId = 0;

Car::Car(float initialSpeed, Travel travel)
	: speed(initialSpeed)
	, travel(std::move(travel))
	, steering(kp, kd)
{
	if (!this->travel.TravelPoints.empty())
		position = this->travel.TravelPoints[0];
	else
		position = Vec2(0.f, 0.f);

	velocity = Vec2(0.f, 0.f);
	acceleration = Vec2(0.f, 0.f);
	id = nextId++;
	travelId = this->travel.getId();
}

// ---------------------------------------------------------------------------
// Public accessors
// ---------------------------------------------------------------------------

int  Car::getId()                  const { return id; }
int  Car::getTravelId()            const { return travelId; }
Vec2 Car::getPosition()            const { return position; }
Vec2 Car::getVelocityVector()      const { return velocity; }
Vec2 Car::getAccelerationVector()  const { return acceleration; }
bool Car::isFinished()             const { return finished; }
int* Car::getColor()          const { return const_cast<int*>(color); }

CarState Car::getState() const
{
	return { position, velocity, acceleration, id };
}

// ---------------------------------------------------------------------------
// Protected path helpers — shared by CarHuman and CarAV
// ---------------------------------------------------------------------------

bool Car::isPathValid() const
{
	return travel.TravelPoints.size() >= 3;
}

bool Car::isFinishedInternal()
{
	if (segment + 2 >= static_cast<int>(travel.TravelPoints.size()))
	{
		finished = true;
		velocity = Vec2(0.f, 0.f);
		return true;
	}
	return false;
}

void Car::updateClosestT()
{
	const auto& p = travel.TravelPoints;

	const Vec2& p0 = p[segment];
	const Vec2& p1 = p[segment + 1];
	const Vec2& p2 = p[segment + 2];

	float bestT = t;
	float bestDist = (travel.bezier(p0, p1, p2, t) - position).length();

	for (int i = -2; i <= 2; ++i)
	{
		float testT = std::clamp(t + i * 0.02f, 0.0f, 1.0f);
		float d = (travel.bezier(p0, p1, p2, testT) - position).length();

		if (d < bestDist)
		{
			bestDist = d;
			bestT = testT;
		}
	}

	t = bestT;
}

bool Car::advanceSegmentIfNeeded()
{
	if (t >= 0.999f)
	{
		segment += 2;
		t = 0.0f;
		return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
// Protected physics — identical for every vehicle type
// ---------------------------------------------------------------------------

void Car::integrate(const Vec2& desiredAcceleration, float dt)
{
	const Vec2 oldVelocity = velocity;

	acceleration = desiredAcceleration;

	float accLen = acceleration.length();
	if (accLen > maxAccel)
		acceleration = acceleration / accLen * maxAccel;

	velocity += acceleration * dt;
	position += velocity * dt;

	speed = velocity.length();

	// Recompute acceleration from velocity delta so getAccelerationVector()
	// always reflects what actually happened this tick.
	if (dt > 0.00001f)
		acceleration = (velocity - oldVelocity) / dt;
	else
		acceleration = Vec2(0.f, 0.f);
}