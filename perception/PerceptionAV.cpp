#include "perception/PerceptionAV.h"
#include "road/Junction.h"

#include <algorithm>
#include <cmath>
#include <limits>

#define DEG2RAD (3.14159265359f / 180.0f)

void PerceptionAV::update(
	const CarState& self,
	const WorldState& world,
	PerceptionState& out
)
{
	out = PerceptionState{};
	updateCarAhead(self, world, out);

	if (world.junction)
	{
		updateBlockHazard(self, world, out);
		updateConflictPoints(self, world, out);
	}
}

void PerceptionAV::updateCarAhead(
	const CarState& self,
	const WorldState& world,
	PerceptionState& out)
{
	FOVResult fov = calculateFOV(self);
	const auto& others = world.vehicleStates;

	const Block* perceptionMask = nullptr;

	if (world.junction)
		perceptionMask = world.junction->getPerceptionMaskForTravel(self.travelId, self.position);

	Vec2 forward = self.forward;
	Vec2 right(-forward.y, forward.x);

	float bestScore = std::numeric_limits<float>::max();
	out.distanceToCarAhead = std::numeric_limits<float>::max();
	out.hasCarAhead = false;

	for (const auto& o : others)
	{
		if (o.id == self.id)
			continue;

		if (perceptionMask)
		{
			if (perceptionMask->containsPoint(o.position))
				continue;
		}

		Vec2  relPos = o.position - self.position;
		float dist = relPos.length();

		if ((dist < 0.001f || dist > fov.maxViewDistance) && o.id != self.id)
			continue;

		Vec2  dir = relPos / dist;

		if (forward.dot(dir) < fov.fovDot)
			continue;

		if (std::fabs(relPos.dot(right)) > kLaneWidth)
			continue;

		Vec2  relVel = o.velocity - self.velocity;
		float closingSpeed = -relVel.dot(dir);

		if (closingSpeed <= 0.1f) {
			if (dist < out.distanceToCarAhead)
				out.distanceToCarAhead = dist;
			continue;
		}

		float ttc = dist / closingSpeed;
		if (ttc < bestScore)
		{
			bestScore = ttc;
			out.hasCarAhead = true;
			out.carAhead = o;
			out.distanceToCarAhead = dist;
			out.relativeSpeed = self.velocity.length() - o.velocity.length();
			out.relativeAcceleration = self.acceleration.length() - o.acceleration.length();
		}
	}

	out.fovDot = fov.fovDot;
	out.maxViewDistance = fov.maxViewDistance;
}

void PerceptionAV::updateBlockHazard(
	const CarState& self,
	const WorldState& world,
	PerceptionState& out
)
{
	const Block* perceptionMask = world.junction->getPerceptionMaskForTravel(self.travelId, self.position);
	const auto& blocks = world.junction->getBlocks();
	if (blocks.empty()) return;

	Vec2 forward = self.forward;
	Vec2 right(-forward.y, forward.x);

	float bestThreat = 0.f;
	int bestIdx = -1;
	float bestDist = 999999.f;

	for (int i = 0; i < static_cast<int>(blocks.size()); ++i)
	{
		if (perceptionMask)
		{
			if (perceptionMask->containsPoint(blocks[i].getCenter()))
			{
				continue;
			}
		}
		Vec2 toBlock = blocks[i].getCenter() - self.position;
		float longitudinal = toBlock.dot(forward);
		float lateral = std::fabs(toBlock.dot(right));

		if (longitudinal < 0.0f)
			continue;

		if (longitudinal > kBlockMaxView)
			continue;

		if (lateral > 0.5f)
			continue;

		if (blocks[i].isActive())
		{
			float threat = std::clamp(1.f - longitudinal / kBlockThreatDist, 0.f, 1.f);

			if (threat > bestThreat)
			{
				bestThreat = threat;
				bestIdx = i;
				bestDist = longitudinal;
			}
		}
	}

	if (bestIdx >= 0)
	{
		out.hasBlockHazard = true;
		out.hazardBlockIndex = bestIdx;
		out.hazardDistance = bestDist;
		out.hazardIsActive = blocks[bestIdx].isActive();
		out.hazardThreat = blocks[bestIdx].isActive() ? bestThreat : 0.f;
	}
}

PerceptionAV::FOVResult PerceptionAV::calculateFOV(
	const CarState& self
) const
{
	float speed = self.velocity.length();

	float speedT = std::clamp(speed / 15.0f, 0.0f, 1.0f);
	float speedFactor = speedT * speedT;

	Vec2  forward = (speed > 0.5f) ?
		self.velocity.normalized() : Vec2(1.f, 0.f);
	Vec2  right(-forward.y, forward.x);
	float turningFactor = 0.0f;

	if (speed > 0.5f)
	{
		float lateralAcc = std::fabs(self.acceleration.dot(right));
		turningFactor = std::clamp(lateralAcc / 5.0f, 0.0f, 1.0f);
	}

	float minAngle = kFovAngleNarrow * DEG2RAD;
	float maxAngle = kFovAngleWide * DEG2RAD;
	float blendT = std::clamp(speedFactor - turningFactor * 0.7f, 0.0f, 1.0f);

	float angle = maxAngle - blendT * (maxAngle - minAngle);
	float fovDot = std::cos(angle);

	float viewT = std::clamp(speed / 20.0f, 0.0f, 1.0f);
	viewT = std::sqrt(viewT);
	float maxViewDistance = kMaxViewDistMin + viewT * (kMaxViewDistMax - kMaxViewDistMin);

	return { fovDot, maxViewDistance };
}

void PerceptionAV::updateConflictPoints(
	const CarState& self,
	const WorldState& world,
	PerceptionState& out)
{
	const auto& cps = world.junction->getConflictPoints();
	float selfSpeed = std::max(self.velocity.length(), 0.5f);

	for (const auto& cp : cps)
	{
		if (!cp.mustYield(self.travelId)) continue;

		Vec2 myToCp = cp.position - self.position;

		// Jeśli punkt jest za nami, to znaczy że już wjechaliśmy/przejechaliśmy - nie hamujemy.
		if (self.forward.dot(myToCp) < -1.5f) continue;

		float myDist = myToCp.length();
		if (myDist > 40.f) continue;

		float myArrival = myDist / selfSpeed;

		for (const auto& o : world.vehicleStates)
		{
			if (o.id == self.id) continue;
			if (!cp.hasPriority(o.travelId)) continue;

			Vec2 otherToCp = cp.position - o.position;

			// Jeśli inny pojazd już przejechał przez swój punkt (z tolerancją -2.0m na długość zderzaka)
			if (o.forward.dot(otherToCp) < -2.0f) continue;

			float otherDist = otherToCp.length();
			if (otherDist > 40.f) continue;

			float otherSpeed = std::max(o.velocity.length(), 0.5f);
			float otherArrival = otherDist / otherSpeed;

			if (otherArrival < myArrival + 1.5f)
			{
				out.hasConflict = true;
				out.conflictingCar = o;
				out.conflictDistance = myDist;
				out.myArrival = myArrival;
				out.otherArrival = otherArrival;
				out.conflictThreat = 1.f - std::clamp(otherArrival / (myArrival + 0.001f), 0.f, 1.f);
				return;
			}
		}
	}
}