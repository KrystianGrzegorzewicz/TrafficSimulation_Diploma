#include "road/Junction.h"
#include <random>
#include <ctime>
#include <iostream>

// ---------------------------------------------------------------------------
// Default constructor — minimal straight-road layout
// ---------------------------------------------------------------------------

Junction::Junction()
{
	blocks.emplace_back(0.0f, -1.75f, 5.0f, 1.75f, 10.0f, 5.0f);

	lines.emplace_back(-100.0f, -1.75f, 100.0f, -1.75f, 0.2f);
	lines.emplace_back(-100.0f, 1.75f, 100.0f, 1.75f, 0.2f);

	std::vector<Vec2> tp = {
		Vec2(-100.f,0.f), Vec2(-100.f,0.f), Vec2(-60.f,0.f),
		Vec2(-30.f, 0.f), Vec2(0.f, 0.f), Vec2(30.f,0.f),
		Vec2(60.f,0.f), Vec2(100.f, 0.f), Vec2(100.f, 0.f)
	};
	travels.emplace_back(tp, 1, 1);
}

// ---------------------------------------------------------------------------
// Indexed constructor — all junction layouts
// ---------------------------------------------------------------------------

Junction::Junction(int index)
{
	std::vector<Vec2> tp1, tp2, tp3, tp4, tp5, tp6;

	switch (index)
	{
		// ------------------------------------------------------------------
	case 0:
		// Default (empty — handled by default ctor logic above)
		break;

		// ------------------------------------------------------------------
	case 1: // Conflict-free right turn
		lines.emplace_back(-100.f, -3.5f, 100.f, -3.5f, 0.3f);
		lines.emplace_back(-100.f, 0.f, 100.f, 0.f, 0.1f);
		lines.emplace_back(-100.f, 3.5f, -3.5f, 3.5f, 0.3f);
		lines.emplace_back(0.f, 3.5f, 100.f, 3.5f, 0.3f);
		lines.emplace_back(-3.5f, 3.5f, -3.5f, 100.f, 0.3f);
		lines.emplace_back(0.f, 3.5f, 0.f, 100.f, 0.3f);

		// Left → right
		tp1 = {
			Vec2(-90.f,1.75f), Vec2(-90.f,1.75f), Vec2(-60.f,1.75f),
			Vec2(-30.f,1.75f), Vec2(-5.f,1.75f), Vec2(5.f,1.75f),
			Vec2(30.f,1.75f), Vec2(60.f,1.75f), Vec2(90.f,1.75f),
			Vec2(90.f,1.75f)
		};
		// Left → down
		tp2 = {
			Vec2(-90.f,1.75f), Vec2(-90.f,1.75f), Vec2(-60.f,1.75f),
			Vec2(-30.f,1.75f), Vec2(-5.f,1.75f), Vec2(-1.75f,1.75f),
			Vec2(-1.75f,5.0f), Vec2(-1.75f,15.f), Vec2(-1.75f,30.f),
			Vec2(-1.75f,45.f), Vec2(-1.75f,45.f)
		};
		// Right → left
		tp3 = {
			Vec2(90.f,-1.75f), Vec2(90.f,-1.75f), Vec2(60.f,-1.75f),
			Vec2(30.f,-1.75f), Vec2(15.f,-1.75f), Vec2(5.f,-1.75f),
			Vec2(-5.f,-1.75f), Vec2(-30.f,-1.75f),Vec2(-60.f,-1.75f),
			Vec2(-90.f,-1.75f), Vec2(-90.f,-1.75f)
		};
		travels.emplace_back(tp1, 2, 1);
		travels.emplace_back(tp2, 2, 2);
		travels.emplace_back(tp3, 1, 3);
		break;

		// ------------------------------------------------------------------
	case 2: // Left turn — one conflict point
		lines.emplace_back(3.5f, 0.f, 3.5f, 3.5f, 0.3f);
		lines.emplace_back(3.5f, 0.f, 100.f, 0.f, 0.3f);
		lines.emplace_back(-100.f, -3.5f, 0.f, -3.5f, 0.3f);
		lines.emplace_back(-100.f, 3.5f, -40.f, 3.5f, 0.3f);
		lines.emplace_back(-100.f, 0.f, -2.f, 0.f, 0.3f);
		lines.emplace_back(-40.f, 3.5f, -26.f, 7.0f, 0.3f);
		lines.emplace_back(-26.f, 7.0f, 10.f, 7.0f, 0.3f);
		lines.emplace_back(10.f, 7.0f, 24.f, 3.5f, 0.3f);
		lines.emplace_back(3.5f, 3.5f, 18.5f, 0.f, 0.3f);
		lines.emplace_back(24.f, 3.5f, 100.f, 3.5f, 0.3f);
		lines.emplace_back(3.5f, -3.5f, 100.f, -3.5f, 0.3f);
		lines.emplace_back(3.5f, -100.f, 3.5f, -3.5f, 0.3f);
		lines.emplace_back(0.f, -100.f, 0.f, -3.5f, 0.3f);
		lines.emplace_back(-20.f, 3.5f, 3.5f, 3.5f, 0.3f);

		// Right → left (straight)
		tp1 = {
			Vec2(100.f,-1.75f), Vec2(100.f,-1.75f), Vec2(90.f,-1.75f),
			Vec2(60.f,-1.75f), Vec2(30.f,-1.75f), Vec2(5.f,-1.75f),
			Vec2(-5.f,-1.75f), Vec2(-30.f,-1.75f), Vec2(-60.f,-1.75f),
			Vec2(-90.f,-1.75f), Vec2(-100.f,-1.75f),Vec2(-100.f,-1.75f)
		};
		// Left → right (via overpass curve)
		tp2 = {
			Vec2(-100.f,1.75f), Vec2(-100.f,1.75f), Vec2(-90.f,1.75f),
			Vec2(-60.f,1.75f), Vec2(-40.f,1.75f), Vec2(-35.f, 1.75f),
			Vec2(-30.f,3.5f),  Vec2(-25.f,5.25f), Vec2(-20.f, 5.25f),
			Vec2(-5.f,5.25f), Vec2(3.5f,5.25f), Vec2(8.5f,5.25f),
			Vec2(13.5f,3.5f), Vec2(18.5f,1.75f), Vec2(23.5f,1.75f),
			Vec2(40.f,1.75f), Vec2(60.f,1.75f), Vec2(90.f,1.75f),
			Vec2(100.f,1.75f), Vec2(100.f,1.75f)
		};
		// Left → up (left turn)
		tp3 = {
			Vec2(-90.f,1.75f),  Vec2(-90.f,1.75f),  Vec2(-60.f,1.75f),
			Vec2(-30.f,1.75f),  Vec2(-3.5f,1.75f), Vec2(1.75f,1.75f),
			Vec2(1.75f,-0.5f),Vec2(1.75f,-30.f), Vec2(1.75f,-60.f),
			Vec2(1.75f,-90.f),Vec2(1.75f,-90.f)
		};
		//circles.append_range(drawTravel(tp3));
		travels.emplace_back(tp1, 5, 1);
		travels.emplace_back(tp2, 5, 2);
		travels.emplace_back(tp3, 1, 3);
		//circles.append_range(drawTravel(tp3));
		break;

		// ------------------------------------------------------------------
	case 3: // Single lane with pedestrian crossing block
		blocks.emplace_back(0.f, -1.75f, 5.f, 1.75f, 10.f, 5.f);

		lines.emplace_back(-100.f, -1.75f, 100.f, -1.75f, 0.2f);
		lines.emplace_back(-100.f, 1.75f, 100.f, 1.75f, 0.2f);

		tp1 = {
			Vec2(-100.f,0.f), Vec2(-100.f,0.f), Vec2(-60.f,0.f),
			Vec2(-30.f,0.f), Vec2(0.f,0.f), Vec2(30.f,0.f),
			Vec2(60.f,0.f), Vec2(100.f,0.f), Vec2(100.f, 0.f)
		};
		travels.emplace_back(tp1, 1, 1);
		break;

		// ------------------------------------------------------------------
	case 4: // Simple roundabout
		circles.emplace_back(0.f, 0.f, 11.f, true);
		circles.emplace_back(0.f, 0.f, 17.f, false);

		// Approach/exit lane markings
		lines.emplace_back(-150.f, -3.5f, -35.f, -3.5f, 0.3f);
		lines.emplace_back(-150.f, 0.f, -25.f, 0.f, 0.1f);
		lines.emplace_back(-150.f, 3.5f, -35.f, 3.5f, 0.3f);
		lines.emplace_back(35.f, -3.5f, 150.f, -3.5f, 0.3f);
		lines.emplace_back(25.f, 0.f, 150.f, 0.f, 0.1f);
		lines.emplace_back(35.f, 3.5f, 150.f, 3.5f, 0.3f);
		lines.emplace_back(-3.5f, -150.f, -3.5f, -35.f, 0.3f);
		lines.emplace_back(0.f, -150.f, 0.f, -25.f, 0.1f);
		lines.emplace_back(3.5f, -150.f, 3.5f, -35.f, 0.3f);
		// Roundabout entry flare lines
		lines.emplace_back(-35.f, -3.5f, -14.5f, -8.8f, 0.3f);
		lines.emplace_back(-35.f, 3.5f, -14.5f, 8.8f, 0.3f);
		lines.emplace_back(-25.f, 0.f, -16.9f, -1.8f, 0.3f);
		lines.emplace_back(-25.f, 0.f, -16.9f, 1.8f, 0.3f);
		lines.emplace_back(35.f, -3.5f, 14.5f, -8.8f, 0.3f);
		lines.emplace_back(35.f, 3.5f, 14.5f, 8.8f, 0.3f);
		lines.emplace_back(25.f, 0.f, 16.9f, -1.8f, 0.3f);
		lines.emplace_back(25.f, 0.f, 16.9f, 1.8f, 0.3f);
		lines.emplace_back(-3.5f, -35.f, -8.8f, -14.5f, 0.2f);
		lines.emplace_back(3.5f, -35.f, 8.8f, -14.5f, 0.2f);
		lines.emplace_back(0.f, -25.f, -1.8f, -16.9f, 0.2f);
		lines.emplace_back(0.f, -25.f, 1.8f, -16.9f, 0.2f);

		// Travel 1: left → right (through roundabout)
		tp1 = {
			Vec2(-150.f,1.75f), Vec2(-150.f,1.75f), Vec2(-100.f,1.75f),
			Vec2(-60.f,1.75f), Vec2(-35.f,1.75f), Vec2(-19.f, 2.f),
			Vec2(-16.2f,  4.f), Vec2(-7.f, 14.f), Vec2(0.f, 14.f),
			Vec2(8.f, 14.f), Vec2(13.f, 7.5f), Vec2(19.f,  2.f),
			Vec2(35.f,1.75f), Vec2(40.f,1.75f), Vec2(60.f,1.75f),
			Vec2(80.f,1.75f), Vec2(100.f,1.75f), Vec2(150.f,1.75f),
			Vec2(150.f,1.75f)
		};
		// Travel 2: left → up (through roundabout, 270° arc)
		tp2 = {
			Vec2(-150.f,1.75f), Vec2(-150.f,1.75f), Vec2(-100.f,1.75f),
			Vec2(-60.f,1.75f), Vec2(-35.f,1.75f), Vec2(-19.f, 2.f),
			Vec2(-16.2f,  4.f), Vec2(-7.f, 14.f), Vec2(0.f, 14.f),
			Vec2(8.f, 14.f), Vec2(12.8f,  7.25f),Vec2(16.f,  0.f),
			Vec2(12.8f, -5.5f), Vec2(10.5f,  -9.f), Vec2(7.f,-12.f),
			Vec2(1.5f,-17.f), Vec2(1.75f,-35.f), Vec2(1.75f,-60.f),
			Vec2(1.75f,-90.f), Vec2(1.75f,-120.f), Vec2(1.75f,-150.f),
			Vec2(1.75f,-150.f)
		};
		// Travel 3: right → up (short arc)
		tp3 = {
			Vec2(150.f,-1.75f), Vec2(150.f,-1.75f), Vec2(120.f,-1.75f),
			Vec2(100.f,-1.75f), Vec2(90.f,-1.75f), Vec2(60.f,-1.75f),
			Vec2(35.f,-1.75f), Vec2(20.f, -2.5f), Vec2(16.2f,  -6.f),
			Vec2(1.75f,-10.f), Vec2(1.75f, -35.f), Vec2(1.75f, -60.f),
			Vec2(1.75f, -90.f), Vec2(1.75f,-120.f), Vec2(1.75f,-150.f),
			Vec2(1.75f,-150.f)
		};
		// Travel 4: right → left (through roundabout)
		tp4 = {
			Vec2(150.f,-1.75f), Vec2(150.f,-1.75f), Vec2(120.f,-1.75f),
			Vec2(100.f,-1.75f), Vec2(90.f,-1.75f), Vec2(60.f,-1.75f),
			Vec2(35.f,-1.75f), Vec2(19.f,  -2.f), Vec2(16.2f,  -4.f),
			Vec2(10.5f,-7.f),  Vec2(6.7f,-11.f),  Vec2(0.f,-16.f),
			Vec2(-8.8f, -9.2f), Vec2(-13.5f, -4.f), Vec2(-35.f,-1.75f),
			Vec2(-60.f,-1.75f), Vec2(-90.f,-1.75f),  Vec2(-120.f,-1.75f),
			Vec2(-150.f,-1.75f), Vec2(-150.f,-1.75f)
		};
		// Travel 5: up → left (arc)
		tp5 = {
			Vec2(-1.75f,-150.f), Vec2(-1.75f,-150.f), Vec2(-1.75f,-120.f),
			Vec2(-1.75f,-100.f), Vec2(-1.75f, -90.f), Vec2(-1.75f, -60.f),
			Vec2(-1.75f, -35.f), Vec2(-1.75f, -22.f), Vec2(-4.6f,-17.f),
			Vec2(-8.6f,  -9.6f),Vec2(-14.f,   -6.f), Vec2(-23.f,-1.75f),
			Vec2(-35.f,-1.75f), Vec2(-60.f, -1.75f), Vec2(-90.f,-1.75f),
			Vec2(-120.f,-1.75f), Vec2(-150.f,-1.75f),  Vec2(-150.f,-1.75f)
		};
		// Travel 6: up → right (270° arc)
		tp6 = {
			Vec2(-1.75f,-150.f), Vec2(-1.75f,-150.f), Vec2(-1.75f, -90.f),
			Vec2(-1.75f, -60.f), Vec2(-1.75f, -35.f), Vec2(-1.75f, -22.f),
			Vec2(-4.6f, -17.f), Vec2(-8.9f,-12.15f),Vec2(-12.f,  -6.6f),
			Vec2(-15.6f,   0.f), Vec2(-11.8f,  6.3f), Vec2(-6.9f, 14.9f),
			Vec2(1.3f, 13.5f), Vec2(10.1f, 12.1f), Vec2(13.f,  7.3f),
			Vec2(19.6f, 1.75f), Vec2(35.f, 1.75f), Vec2(40.f, 1.75f),
			Vec2(60.f, 1.75f), Vec2(80.f, 1.75f), Vec2(100.f, 1.75f),
			Vec2(150.f, 1.75f), Vec2(150.f, 1.75f)
		};

		//circles.append_range(drawTravel(tp3));
		travels.emplace_back(tp1, 1, 1);
		travels.emplace_back(tp2, 1, 2);
		travels.emplace_back(tp3, 1, 3);
		travels.emplace_back(tp4, 1, 4);
		travels.emplace_back(tp5, 1, 5);
		travels.emplace_back(tp6, 1, 6);

		break;

		// ------------------------------------------------------------------
	default:
		std::cerr << "Junction: unknown index " << index << "\n";
		break;
	}
}

// ---------------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------------

std::vector<Block>& Junction::getBlocks()
{
	return blocks;
}

const std::vector<Block>& Junction::getBlocks() const
{
	return blocks;
}

std::vector<Line> Junction::getLines() const
{
	return lines;
}

std::vector<Circle> Junction::getCircles() const
{
	return circles;
}

// ---------------------------------------------------------------------------
// Random weighted travel selection
// ---------------------------------------------------------------------------

Travel Junction::getRandomTravel()
{
	if (travels.empty())
	{
		std::cerr << "Junction: no travels defined\n";
		return Travel({ Vec2(0,0), Vec2(0,0), Vec2(0,0) }, 1, 0);
	}

	int total = 0;
	for (const auto& tr : travels)
		total += tr.getWeight();

	srand(time(0));
	int rnd = rand() % total;
	for (const auto& tr : travels)
	{
		if (rnd < tr.getWeight()) return tr;
		rnd -= tr.getWeight();
	}

	return travels.back();   // unreachable, but satisfies the compiler
}
std::vector<Circle> Junction::drawTravel(std::vector<Vec2> travelPoints) {
	Travel tempTravel(travelPoints, 1, 0);
	const float STEP = 0.05f; // gęstość punktów
	std::vector<Circle> circles;
	for (const Vec2& point : travelPoints) {
		circles.push_back(Circle(point.x, point.y, 0.5f, false));
	}
	for (size_t i = 0; i + 2 < travelPoints.size(); i += 2)
	{
		Vec2 p0 = travelPoints[i];
		Vec2 p1 = travelPoints[i + 1];
		Vec2 p2 = travelPoints[i + 2];
		for (float t = 0.0f; t <= 1.0f; t += STEP)
		{
			Vec2 point = tempTravel.bezier(p0, p1, p2, t);

			circles.push_back(
				Circle(point.x, point.y, 0.3f, false)
			);
		}
	}
	return circles;
}