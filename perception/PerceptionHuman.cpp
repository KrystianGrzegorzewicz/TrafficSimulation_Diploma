#include "perception/PerceptionHuman.h"
#include "road/Junction.h"
#include <algorithm>
#include <cmath>
#include <limits>

#define DEG2RAD (3.14159265359f / 180.0f)

// ---------------------------------------------------------------------------
// Public entry point
// ---------------------------------------------------------------------------

void PerceptionHuman::update(
    const CarState& self,
    const WorldState& world,
    PerceptionState& out
)
{
    out = PerceptionState{};   // reset all fields to defaults

    updateCarAhead(self, world.vehicleStates, out);

    if (world.junction)
        updateBlockHazard(self, world, out);
}

// ---------------------------------------------------------------------------
// Car-ahead detection (TTC-scored, dynamic FOV + lane filter)
// ---------------------------------------------------------------------------

void PerceptionHuman::updateCarAhead(
    const CarState& self,
    const std::vector<CarState>& others,
    PerceptionState& out
)
{
    FOVResult fov = calculateFOV(self);

    Vec2 forward = (self.velocity.length() > 0.5f)
        ? self.velocity.normalized()
        : Vec2(1.f, 0.f);
    Vec2 right(-forward.y, forward.x);

    float bestScore = std::numeric_limits<float>::max();

    for (const auto& o : others)
    {
        Vec2  relPos = o.position - self.position;
        float dist = relPos.length();

        // Skip self (same position) and out-of-range
        if (dist < 0.001f || dist > fov.maxViewDistance)
            continue;

        Vec2  dir = relPos / dist;

        // FOV cone
        if (forward.dot(dir) < fov.fovDot)
            continue;

        // Lane filter
        if (std::fabs(relPos.dot(right)) > kLaneWidth)
            continue;

        // Only approach if the other vehicle is actually closing
        Vec2  relVel = o.velocity - self.velocity;
        float closingSpeed = -relVel.dot(dir);
        if (closingSpeed <= 0.1f)
            continue;

        // TTC score — lower is more urgent
        float ttc = dist / closingSpeed;
        if (ttc < bestScore)
        {
            bestScore = ttc;
            out.hasCarAhead = true;
            out.carAhead = o;
            out.distanceToCarAhead = dist;
            out.relativeSpeed =
                self.velocity.length() - o.velocity.length();
            out.relativeAcceleration =
                self.acceleration.length() - o.acceleration.length();
        }
    }

    // Store FOV params so callers can inspect them if needed
    out.fovDot = fov.fovDot;
    out.maxViewDistance = fov.maxViewDistance;
}

// ---------------------------------------------------------------------------
// Block hazard detection (replaces old BlockPerception class entirely)
// ---------------------------------------------------------------------------

void PerceptionHuman::updateBlockHazard(
    const CarState& self,
    const WorldState& world,
    PerceptionState& out
)
{
    const auto& blocks = world.junction->getBlocks();
    if (blocks.empty()) return;

    Vec2 forward = (self.velocity.length() > 0.5f)
        ? self.velocity.normalized()
        : Vec2(1.f, 0.f);

    float bestThreat = 0.f;
    int   bestIdx = -1;
    float bestDist = 999999.f;

    for (int i = 0; i < static_cast<int>(blocks.size()); ++i)
    {
        Vec2  toBlock = blocks[i].getCenter() - self.position;
        float dist = toBlock.length();

        if (dist > kBlockMaxView) continue;

        // Ignore blocks behind or far to the side
        if (dist > 0.1f && forward.dot(toBlock / dist) < 0.3f) continue;

        if (blocks[i].isActive())
        {
            float threat = std::clamp(
                1.f - (dist / kBlockThreatDist), 0.f, 1.f);

            if (threat > bestThreat ||
                (bestIdx >= 0 && !blocks[bestIdx].isActive()))
            {
                bestThreat = threat;
                bestIdx = i;
                bestDist = dist;
            }
        }
        else if (bestIdx < 0 && dist < bestDist)
        {
            // Track closest inactive block as fallback
            bestIdx = i;
            bestDist = dist;
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

// ---------------------------------------------------------------------------
// Dynamic FOV calculation (ported from old Perception.cpp::calculateFOV)
// ---------------------------------------------------------------------------

PerceptionHuman::FOVResult PerceptionHuman::calculateFOV(
    const CarState& self
) const
{
    float speed = self.velocity.length();

    // Speed factor: 0 at standstill → 1 at 15 m/s, with quadratic roll-off
    float speedT = std::clamp(speed / 15.0f, 0.0f, 1.0f);
    float speedFactor = speedT * speedT;

    // Turning factor: high lateral acceleration → widen cone for awareness
    Vec2  forward = (speed > 0.5f) ? self.velocity.normalized() : Vec2(1.f, 0.f);
    Vec2  right(-forward.y, forward.x);
    float turningFactor = 0.0f;

    if (speed > 0.5f)
    {
        float lateralAcc = std::fabs(self.acceleration.dot(right));
        turningFactor = std::clamp(lateralAcc / 5.0f, 0.0f, 1.0f);
    }

    // Blend between wide (low speed) and narrow (high speed) cone
    float minAngle = kFovAngleNarrow * DEG2RAD;
    float maxAngle = kFovAngleWide * DEG2RAD;

    float blendT = std::clamp(speedFactor - turningFactor * 0.7f, 0.0f, 1.0f);
    float angle = maxAngle - blendT * (maxAngle - minAngle);

    float fovDot = std::cos(angle);

    // View distance: grows with speed (sqrt scaling)
    float viewT = std::clamp(speed / 20.0f, 0.0f, 1.0f);
    viewT = std::sqrt(viewT);
    float maxViewDistance =
        kMaxViewDistMin + viewT * (kMaxViewDistMax - kMaxViewDistMin);

    return { fovDot, maxViewDistance };
}