#include "perception/PerceptionHuman.h"
#include "road/Junction.h"

#include <algorithm>
#include <cmath>
#include <limits>

#define DEG2RAD (3.14159265359f / 180.0f)

void PerceptionHuman::update(
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
		analyzeConflictPoints(self, world, out);
	}
}

void PerceptionHuman::updateCarAhead(
	const CarState& self,
	const WorldState& world,
	PerceptionState& out
)
{
	FOVResult fov = calculateFOV(self);
	const Block* perceptionMask = nullptr;

	if (world.junction)
		perceptionMask = world.junction->getPerceptionMaskForTravel(self.travelId, self.position);
	const auto& others = world.vehicleStates;

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

void PerceptionHuman::updateBlockHazard(
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

PerceptionHuman::FOVResult PerceptionHuman::calculateFOV(const CarState& self) const
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

static float calculateKinematicTime(
	float distance,
	float speed,
	float acceleration)
{
	constexpr float INF = 999999.f;

	if (distance <= 0.0f)
		return 0.0f;

	speed = std::max(speed, 0.0f);

	if (std::fabs(acceleration) < 0.01f)
	{
		if (speed < 0.05f)
			return INF;

		return distance / speed;
	}

	const float discriminant =
		speed * speed +
		2.0f * acceleration * distance;

	if (discriminant < 0.0f)
		return INF;

	const float finalSpeed = std::sqrt(discriminant);

	const float time =
		(finalSpeed - speed) / acceleration;

	if (time < 0.0f)
		return INF;

	return time;
}

void PerceptionHuman::analyzeConflictPoints(
	const CarState& self,
	const WorldState& world,
	PerceptionState& out)
{
	out.hasConflict = false;
	out.alreadyEnteringConflict = false;

	out.conflictDistance = 999999.f;
	out.myArrival = 999999.f;
	out.otherArrival = 999999.f;
	out.conflictThreat = 0.f;

	out.selfTtaEntry = 999999.f;
	out.selfTtaExit = 999999.f;

	out.conflictingCar = CarState{};
	out.priorityCarsTTA.clear();

	if (!world.junction)
		return;

	const auto& conflictPoints =
		world.junction->getConflictPoints();

	const FOVResult fov = calculateFOV(self);

	const float selfSpeed = self.velocity.length();

	for (const auto& cp : conflictPoints)
	{
		if (!cp.mustYield(self.travelId))
			continue;

		const Vec2 toConflict =
			cp.position - self.position;

		const float distance =
			toConflict.length();

		if (self.forward.dot(toConflict) < -cp.radius)
			continue;

		if (distance > fov.maxViewDistance)
			continue;

		if (distance > 0.001f)
		{
			const Vec2 direction =
				toConflict.normalized();

			if (self.forward.dot(direction) < fov.fovDot)
				continue;
		}

		if (distance <= cp.radius)
		{
			out.alreadyEnteringConflict = true;
			continue;
		}
		float kConflictRadius = cp.radius;
		const float entryDistance =
			std::max(
				0.0f,
				distance - kConflictRadius
			);

		const float exitDistance =
			distance + kConflictRadius;

		float selfAcceleration =
			self.acceleration.dot(self.forward);

		float planningSpeed = selfSpeed;

		if (planningSpeed < 0.5f)
		{
			planningSpeed = 0.0f;
			selfAcceleration = 2.5f;
		}
		else
		{
			selfAcceleration =
				std::max(selfAcceleration, 0.0f);
		}

		out.selfTtaEntry =
			calculateKinematicTime(
				entryDistance,
				planningSpeed,
				selfAcceleration
			);

		out.selfTtaExit =
			calculateKinematicTime(
				exitDistance,
				planningSpeed,
				selfAcceleration
			);

		for (const auto& other : world.vehicleStates)
		{
			if (other.id == self.id)
				continue;

			if (!cp.hasPriority(other.travelId))
				continue;

			const Vec2 otherToConflict =
				cp.position - other.position;

			const float otherDistance =
				otherToConflict.length();

			if (other.forward.dot(otherToConflict) < -cp.radius)
				continue;

			if (otherDistance > fov.maxViewDistance)
				continue;

			if (otherDistance > 0.001f)
			{
				const Vec2 direction =
					(other.position - self.position).normalized();

				if (self.forward.dot(direction) < fov.fovDot)
					continue;
			}

			const float otherEntryDistance =
				std::max(
					0.0f,
					otherDistance - kConflictRadius
				);

			const float otherExitDistance =
				otherDistance + kConflictRadius;

			float otherSpeed =
				other.velocity.length();

			float otherAcceleration =
				other.acceleration.dot(other.forward);

			if (otherSpeed < 0.05f)
			{
				otherAcceleration = 0.0f;
			}

			PriorityCarTTA tta;

			tta.carId = other.id;
			tta.distanceToCp = otherDistance;
			tta.isAV = other.isAV;

			tta.ttaEntry =
				calculateKinematicTime(
					otherEntryDistance,
					otherSpeed,
					otherAcceleration
				);

			tta.ttaExit =
				calculateKinematicTime(
					otherExitDistance,
					otherSpeed,
					otherAcceleration
				);

			out.priorityCarsTTA.push_back(tta);
		}

		if (out.priorityCarsTTA.empty())
			continue;

		std::sort(
			out.priorityCarsTTA.begin(),
			out.priorityCarsTTA.end(),
			[](const PriorityCarTTA& a,
				const PriorityCarTTA& b)
			{
				return a.ttaEntry < b.ttaEntry;
			}
		);

		for (const auto& candidate :
			out.priorityCarsTTA)
		{
			if (candidate.ttaEntry >= 999998.f)
				continue;

			const bool temporalOverlap =
				out.selfTtaEntry < candidate.ttaExit &&
				candidate.ttaEntry < out.selfTtaExit;

			if (!temporalOverlap)
				continue;

			for (const auto& other :
				world.vehicleStates)
			{
				if (other.id != candidate.carId)
					continue;

				out.hasConflict = true;

				out.conflictingCar = other;

				out.conflictDistance =
					distance;

				out.myArrival =
					out.selfTtaEntry;

				out.otherArrival =
					candidate.ttaEntry;

				out.otherTtaEntry =
					candidate.ttaEntry;

				out.otherTtaExit =
					candidate.ttaExit;

				const float timeDifference =
					std::fabs(
						out.selfTtaEntry -
						candidate.ttaEntry
					);

				out.conflictThreat =
					1.0f -
					std::clamp(
						timeDifference / 5.0f,
						0.0f,
						1.0f
					);

				break;
			}

			if (out.hasConflict)
				break;
		}

		if (out.hasConflict)
			return;
	}
}