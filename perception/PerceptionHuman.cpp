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
		updateConflictPoints(self, world, out);
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

void PerceptionHuman::updateConflictPoints(const CarState& self, const WorldState& world, PerceptionState& out)
{
	const auto& cps = world.junction->getConflictPoints();
	float selfSpeed = std::max(self.velocity.length(), 0.5f);

	float riskFactor = personality.gapFactor;
	float aggressivenessMargin = 2.5f - (riskFactor * 1.5f);

	for (const auto& cp : cps)
	{
		if (!cp.mustYield(self.travelId)) continue;

		Vec2 myToCp = cp.position - self.position;
		if (self.forward.dot(myToCp) < -1.5f) continue;

		float myDist = myToCp.length();
		if (myDist > 40.0f) continue;

		float myArrival = (myDist / selfSpeed) * riskFactor;

		for (const auto& o : world.vehicleStates)
		{
			if (o.id == self.id) continue;
			if (!cp.hasPriority(o.travelId)) continue;

			Vec2 otherToCp = cp.position - o.position;

			// Zabezpieczenie 1: czy fizycznie minął już punkt?
			if (o.forward.dot(otherToCp) < -2.0f) continue;

			float otherDist = otherToCp.length();
			if (otherDist < 0.1f) continue; // Unikamy dzielenia przez zero

			// --- NOWA LOGIKA: PRĘDKOŚĆ ZBLIŻANIA ---
			Vec2 dirToCp = otherToCp / otherDist; // Znormalizowany wektor kierunku do punktu
			float approachSpeed = o.velocity.dot(dirToCp);

			// Jeśli pojazd nie zbliża się w naszą stronę (np. jest po drugiej stronie ronda
			// lub zjeżdża) to jego approachSpeed będzie małe albo ujemne.
			// Dodajemy warunek otherDist > 5.0f, żeby nie ignorować kogoś, kto fizycznie
			// blokuje już przejazd tuż przed naszym zderzakiem.
			if (approachSpeed < 0.5f && otherDist > 5.0f)
			{
				continue; // Ignorujemy ten samochód, bo "nie patrzy/nie jedzie" w stronę punktu
			}

			// Obliczamy czas przyjazdu tylko na podstawie TEJ części prędkości,
			// która faktycznie przybliża go do kolizji.
			float effectiveSpeed = std::max(approachSpeed, 0.5f);
			float otherArrival = otherDist / effectiveSpeed;
			// ---------------------------------------

			// LOGIKA HISTEREZY:
			float hysteresisMargin = out.alreadyEnteringConflict ? (aggressivenessMargin + 1.0f) : aggressivenessMargin;

			if (otherArrival < myArrival + hysteresisMargin)
			{
				out.hasConflict = true;
				out.alreadyEnteringConflict = false; // Wycofujemy się z zamiaru wjazdu
				return;
			}
		}
	}

	// Jeśli pętla przeszła i nie wykryto blokującego konfliktu:
	out.hasConflict = false;
	out.alreadyEnteringConflict = true; // Potwierdzamy, że droga wolna, możemy wjechać
}