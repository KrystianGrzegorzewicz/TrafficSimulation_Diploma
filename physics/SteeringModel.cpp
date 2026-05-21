#include "physics/SteeringModel.h"
#include <algorithm>

Vec2 SteeringModel::computeLateralAcceleration(
	const Vec2& position,
	const Vec2& velocity,
	const Vec2& targetPoint,
	const Vec2& targetTangent) const
{
	float speed =
		velocity.length();

	Vec2 toTarget =
		targetPoint -
		position;

	if (toTarget.lengthSquared() < 1e-6f)
	{
		return Vec2(0, 0);
	}

	Vec2 forward =
		(speed > 0.1f)
		? velocity.normalized()
		: toTarget.normalized();

	Vec2 right(
		-forward.y,
		forward.x);

	float lateralError =
		toTarget.dot(right);

	float lateralVelocity =
		velocity.dot(right);

	float headingError =
		forward.cross(targetTangent);

	float commandedLatAcc =
		kp * lateralError
		+ headingKp * headingError
		- kd * lateralVelocity;

	float speedGain =
		std::clamp(
			speed / 12.0f,
			0.5f,
			1.4f);

	commandedLatAcc *=
		speedGain;

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

	commandedLatAcc =
		std::clamp(
			commandedLatAcc,
			-aLatMax,
			aLatMax);

	return right *
		commandedLatAcc;
}