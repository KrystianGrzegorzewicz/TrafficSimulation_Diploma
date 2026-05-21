#include "vehicles/Car.h"
#include "perception/IPerception.h"
#include <algorithm>
#include <cmath>

int Car::nextId = 0;
Car::~Car() = default;
Car::Car(float initialSpeed, Travel travelIn)
	: travel(std::move(travelIn))
	, steering(kp, kd)
{
	id = nextId++;
	travelId = travel.getId();

	segment = 0;
	t = 0.0f;
	finished = false;

	const auto& pts = travel.TravelPoints;

	if (!pts.empty()) {
		position = pts[0];
	}
	else {
		position = Vec2(0.f, 0.f);
	}

	Vec2 tangent(1.f, 0.f);

	if (pts.size() >= 3) {
		tangent = travel.bezierDerivative(
			pts[0],
			pts[1],
			pts[2],
			0.0f
		);

		if (tangent.length() < 0.0001f) {
			tangent = pts[1] - pts[0];
		}

		if (tangent.length() < 0.0001f) {
			tangent = pts[2] - pts[0];
		}

		tangent.normalize();
	}

	// Important:
	// zero speed means zero velocity, and zero velocity has no direction.
	// So give every newly spawned car a tiny minimum launch speed.
	const float minLaunchSpeed = 2.0f;

	speed = std::max(initialSpeed, minLaunchSpeed);
	velocity = tangent * speed;

	acceleration = Vec2(0.f, 0.f);
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
	return { position, velocity, acceleration, travelId, id };
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

	if (segment + 2 >= (int)p.size())
		return;

	const Vec2& p0 = p[segment];
	const Vec2& p1 = p[segment + 1];
	const Vec2& p2 = p[segment + 2];

	// --- adaptive search window ---
	float speed = velocity.length();

	// większe okno przy dużej prędkości
	float searchRadiusT =
		std::clamp(0.08f + speed * 0.008f,
			0.08f,
			0.35f);

	float startT =
		std::max(0.0f, t - searchRadiusT);

	float endT =
		std::min(1.0f, t + searchRadiusT);

	float bestT = t;
	float bestDist2 = FLT_MAX;

	// coarse search
	const int coarseSteps = 30;

	for (int i = 0; i <= coarseSteps; ++i)
	{
		float testT =
			startT +
			(endT - startT) *
			(i / (float)coarseSteps);

		Vec2 curvePoint =
			travel.bezier(p0, p1, p2, testT);

		float d2 =
			(curvePoint - position)
			.lengthSquared();

		if (d2 < bestDist2)
		{
			bestDist2 = d2;
			bestT = testT;
		}
	}

	// fine refinement
	float refineRange = 0.03f;

	for (int iter = 0; iter < 3; ++iter)
	{
		float localBestT = bestT;
		float localBestD2 = bestDist2;

		for (int i = -8; i <= 8; ++i)
		{
			float testT =
				std::clamp(
					bestT + i * refineRange / 8.f,
					0.f,
					1.f
				);

			Vec2 pt =
				travel.bezier(
					p0, p1, p2, testT);

			float d2 =
				(pt - position)
				.lengthSquared();

			if (d2 < localBestD2)
			{
				localBestD2 = d2;
				localBestT = testT;
			}
		}

		bestT = localBestT;
		bestDist2 = localBestD2;
		refineRange *= 0.5f;
	}

	t = bestT;
}

bool Car::advanceSegmentIfNeeded()
{
	if (t >= 0.999f)
	{
		segment += 2;
		t = 0.0f;

		if (segment + 2 >= static_cast<int>(travel.TravelPoints.size()))
		{
			finished = true;
			velocity = Vec2(0.f, 0.f);
			return false;
		}

		return true;
	}

	return false;
}

// ---------------------------------------------------------------------------
// Protected physics — identical for every vehicle type
// ---------------------------------------------------------------------------

void Car::integrate(
	const Vec2& desiredAcceleration,
	float dt)
{
	const Vec2 oldVelocity = velocity;

	Vec2 forward =
		velocity.length() > 0.1f
		? velocity.normalized()
		: Vec2(1, 0);

	Vec2 right(-forward.y, forward.x);

	float aLong =
		desiredAcceleration.dot(forward);

	float aLat =
		desiredAcceleration.dot(right);

	// separate limits
	aLong = std::clamp(
		aLong,
		-maxDecel,
		maxAccel);

	// friction circle
	const float tireGrip = 9.0f;

	float maxLatAvailable =
		std::sqrt(std::max(
			0.f,
			tireGrip * tireGrip
			- aLong * aLong));

	aLat = std::clamp(
		aLat,
		-maxLatAvailable,
		maxLatAvailable);

	acceleration =
		forward * aLong +
		right * aLat;

	// semi-implicit Euler
	velocity += acceleration * dt;

	float speed = velocity.length();

	if (speed > maxSpeed)
	{
		velocity =
			velocity.normalized()
			* maxSpeed;
	}

	position += velocity * dt;

	this->speed =
		velocity.length();

	if (dt > 1e-5f)
	{
		acceleration =
			(velocity - oldVelocity)
			/ dt;
	}
}
void Car::executeUpdate(
	float dt,
	const WorldState& world,
	IBehavior& behavior,
	IPerception& perception,
	PerceptionState& perceptionState)
{
	if (finished)
		return;

	if (!isPathValid())
	{
		finished = true;
		return;
	}

	updateClosestT();

	advanceSegmentIfNeeded();

	if (isFinishedInternal())
		return;

	CarState self = getState();

	perception.update(
		self,
		world,
		perceptionState);

	MotionCommand cmd =
		behavior.compute(
			travel,
			segment,
			t,
			self,
			maxSpeed,
			maxAccel,
			maxDecel,
			lookaheadBase,
			lookaheadSpeedFactor,
			perceptionState);

	Vec2 lateralAcc =
		steering.computeLateralAcceleration(
			position,
			velocity,
			cmd.targetPoint);

	Vec2 forward =
		velocity.length() > 0.1f
		? velocity.normalized()
		: Vec2(1.f, 0.f);

	Vec2 longitudinalAcc =
		forward * cmd.longitudinalAcceleration;

	Vec2 totalAcc =
		lateralAcc + longitudinalAcc;

	if (cmd.emergencyBrake)
	{
		totalAcc =
			forward * (-maxDecel);
	}

	integrate(totalAcc, dt);
}