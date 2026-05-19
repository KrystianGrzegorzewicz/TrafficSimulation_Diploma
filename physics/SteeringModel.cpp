#include "physics/SteeringModel.h"
#include <algorithm>

Vec2 SteeringModel::computeLateralAcceleration(
	const Vec2& position,
	const Vec2& velocity,
	const Vec2& targetPoint
) const
{
	float speed = velocity.length();

	Vec2 toTargetRaw = targetPoint - position;
	Vec2 forward =
		(speed > 0.1f)
		? velocity.normalized()
		: (toTargetRaw.length() > 0.001f
			? toTargetRaw.normalized()
			: Vec2(1.f, 0.f));

	Vec2 right(-forward.y, forward.x);

	Vec2 toTarget = targetPoint - position;

	float forwardMag = toTarget.dot(forward);

	Vec2 lateralError =
		toTarget - forward * forwardMag;

	float lateralVelocity =
		velocity.dot(right);

	// PD controller
	// a = kp * error - kd * velocity
	Vec2 aLateral =
		lateralError * kp -
		right * lateralVelocity * kd;

	// Physics cornering limit
	// v² / R
	float safeSpeed =
		std::max(speed * 0.9f, 0.1f);

	float maxFromRadius =
		(safeSpeed * safeSpeed) /
		minTurnRadius;

	float maxLatAcc =
		std::min(aLatMax, maxFromRadius);

	float len = aLateral.length();

	if (len > maxLatAcc)
	{
		aLateral =
			aLateral / len * maxLatAcc;
	}

	// Reduce oscillations at low speed
	float damping =
		std::clamp(speed / 5.0f, 0.0f, 1.0f);

	return aLateral;
}