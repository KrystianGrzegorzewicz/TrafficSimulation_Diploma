#include "physics/SteeringModel.h"
#include <algorithm>

Vec2 SteeringModel::computeLateralAcceleration(
	const Vec2& position,
	const Vec2& velocity,
	const Vec2& targetPoint
) const
{
	float speed = velocity.length();

	Vec2 toTarget =
		targetPoint - position;

	if (toTarget.lengthSquared() < 1e-6f)
		return Vec2(0, 0);

	Vec2 forward =
		(speed > 0.1f)
		? velocity.normalized()
		: toTarget.normalized();

	Vec2 right(-forward.y, forward.x);

	float lateralError =
		toTarget.dot(right);

	float lateralVelocity =
		velocity.dot(right);

	// critically damped PD
	float omega = kp;

	float commandedLatAcc =
		omega * omega * lateralError
		- 2.f * kd * lateralVelocity;

	// ograniczenie wyłącznie geometrią skrętu
	if (speed > 0.5f)
	{
		float maxCurvatureAcc =
			(speed * speed)
			/ minTurnRadius;

		commandedLatAcc =
			std::clamp(
				commandedLatAcc,
				-maxCurvatureAcc,
				maxCurvatureAcc);
	}

	return right * commandedLatAcc;
}