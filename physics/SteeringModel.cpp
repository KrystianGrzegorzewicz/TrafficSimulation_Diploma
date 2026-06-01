#include "physics/SteeringModel.h"
#include <algorithm>

Vec2 SteeringModel::computeLateralAcceleration(
	const Travel& travel,
	int segment,
	float t,
	const Vec2& position,
	const Vec2& velocity,
	float lookaheadBase,
	float lookaheadSpeedFactor,
	const MotionCommand& cmd
) const
{
	const auto& p = travel.TravelPoints;

	if (segment + 2 >= (int)p.size())
	{
		return Vec2(0.f, 0.f);
	}

	Vec2 p0 = p[segment];
	Vec2 p1 = p[segment + 1];
	Vec2 p2 = p[segment + 2];

	float speed = velocity.length();

	float lookahead =
		lookaheadBase +
		speed * lookaheadSpeedFactor;

	float tLook =
		std::min(t + lookahead, 1.0f);

	Vec2 tangent =
		travel.bezierDerivative(
			p0,
			p1,
			p2,
			tLook);

	Vec2 forward =
		tangent.normalized();

	Vec2 right(
		-forward.y,
		forward.x);

	Vec2 toTarget =
		cmd.targetPoint - position;

	float forwardMag =
		toTarget.dot(forward);

	Vec2 lateralError =
		toTarget -
		forward * forwardMag;

	float forwardVel =
		velocity.dot(forward);

	if (forwardVel < 0.0f)
	{
		forwardVel = 0.0f;
	}

	float lateralVel =
		velocity.dot(right);

	Vec2 a_lateral =
		lateralError * kp
		- right * lateralVel * kd;

	float safeSpeed =
		std::max(
			forwardVel,
			0.1f);

	float maxFromRadius =
		(safeSpeed * safeSpeed)
		/ minTurnRadius;

	float maxLatAcc =
		std::min(
			aLatMax,
			maxFromRadius);

	float latLen =
		a_lateral.length();

	if (latLen > maxLatAcc)
	{
		a_lateral =
			a_lateral / latLen
			* maxLatAcc;
	}

	float lowSpeedDamping =
		std::clamp(
			speed / 5.0f,
			0.0f,
			1.0f);

	a_lateral *=
		lowSpeedDamping;

	return a_lateral;
}