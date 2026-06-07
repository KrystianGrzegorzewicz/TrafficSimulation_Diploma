#include "physics/SteeringModel.h"

#include <algorithm>
#include <iostream>

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
		return Vec2(0.f, 0.f);

	//Path geometry
	Vec2 p0 = p[segment];
	Vec2 p1 = p[segment + 1];
	Vec2 p2 = p[segment + 2];

	Vec2 pathPoint = travel.bezier(p0, p1, p2, t);
	Vec2 tangent = travel.bezierDerivative(p0, p1, p2, t).normalized();
	Vec2 right(-tangent.y, tangent.x);

	//Cross-track error
	Vec2 toPath = pathPoint - position;
	float e_lat = toPath.dot(right);

	//Heading error
	Vec2 velDir =
		velocity.length() > 0.1f
		? velocity.normalized()
		: tangent;

	float e_heading = velDir.cross(tangent);

	//Lateral velocity (derivative term)
	float e_lat_dot = velocity.dot(right);

	//True PD control
	float a_lat = kp * e_lat - kd * e_lat_dot + 1 * e_heading;

	//Clamp by physical limits
	float speed = velocity.length();
	float maxFromRadius = (speed * speed) / minTurnRadius;
	float maxLatAcc = std::min(aLatMax, maxFromRadius);

	if (std::fabs(a_lat) > maxLatAcc)
		a_lat = (a_lat > 0 ? 1.f : -1.f) * maxLatAcc;

	//Low-speed damping
	float lowSpeedDamping = std::clamp(speed / 5.0f, 0.0f, 1.0f);
	a_lat *= lowSpeedDamping;

	return right * a_lat;
}