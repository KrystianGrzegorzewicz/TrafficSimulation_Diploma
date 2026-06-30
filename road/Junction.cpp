#include "road/Junction.h"

#include <random>
#include <ctime>
#include <iostream>

Junction::Junction()
{
	blocks.push_back(Block(-100.0f, -100.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, {}));

	blocks.emplace_back(0.0f, -1.75f, 5.0f, 1.75f, 10.0f, 5.0f, 0.0f);
	//Block b(-100.0f, -100.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::Visual, {});
	//blocks.emplace_back(-100.0f, -100.0f,100.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::Visual, {});
	//blocks.push_back(b);

	lines.emplace_back(-100.0f, -1.75f, 100.0f, -1.75f, 0.2f);
	lines.emplace_back(-100.0f, 1.75f, 100.0f, 1.75f, 0.2f);

	std::vector<Vec2> tp = {
		Vec2(-100.f,0.f), Vec2(-100.f,0.f), Vec2(-60.f,0.f),
		Vec2(-30.f, 0.f), Vec2(0.f, 0.f), Vec2(30.f,0.f),
		Vec2(60.f,0.f), Vec2(100.f, 0.f), Vec2(100.f, 0.f)
	};
	travels.emplace_back(tp, 1, 1);
}

Junction::Junction(int index)
{
	std::vector<Vec2> tp1, tp2, tp3, tp4, tp5, tp6, tp7, tp8, tp9, tp10, tp11, tp12, tp13, tp14;
	float lightCycle, bufforTime;
	ConflictPoint c;

	switch (index)
	{
		// ------------------------------------------------------------------
	case 0:
		break;

		// ------------------------------------------------------------------
	case 1: // Conflict-free right turn
		//mask
		blocks.push_back(Block(-100.0f, -50.0f, 100.0f, 50.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, {}));

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
			Vec2(90.f,-1.75f), Vec2(85.f,-1.75f), Vec2(60.f,-1.75f),
			Vec2(30.f,-1.75f), Vec2(15.f,-1.75f), Vec2(5.f,-1.75f),
			Vec2(-5.f,-1.75f), Vec2(-30.f,-1.75f),Vec2(-60.f,-1.75f),
			Vec2(-90.f,-1.75f), Vec2(-90.f,-1.75f)
		};
		travels.emplace_back(tp1, 0, 1);
		travels.emplace_back(tp2, 1, 2);
		travels.emplace_back(tp3, 0, 3);
		circles.append_range(drawTravel(tp2));
		break;

		// ------------------------------------------------------------------
	case 2: // Left turn — one conflict point
		//mask
		//blocks.push_back(Block(-100.0f, -100.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, {}));
		blocks.push_back(Block(-100.0f, 0.0f, 100.0f, 10.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 1 }));

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
			Vec2(100.f,-1.75f), Vec2(99.f,-1.75f), Vec2(90.f,-1.75f),
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
			Vec2(1.75f,-3.5f),Vec2(1.75f,-30.f), Vec2(1.75f,-60.f),
			Vec2(1.75f,-90.f),Vec2(1.75f,-90.f)
		};
		//circles.append_range(drawTravel(tp3));
		travels.emplace_back(tp1, 5, 1);
		travels.emplace_back(tp2, 5, 2);
		travels.emplace_back(tp3, 2, 3);

		c.position = Vec2(1.75f, -1.75f);
		c.priorityTravels = { 1 };
		c.yieldTravels = { 3 };
		c.radius = 4.f;
		conflictPoints.push_back(c);
		circles.emplace_back(c.position.x, c.position.y, c.radius, true);

		break;

		// ------------------------------------------------------------------
	case 3: // Single lane with pedestrian crossing block
		//mask
		blocks.push_back(Block(-300.0f, -100.0f, 300.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, {}));

		blocks.emplace_back(0.f, -1.75f, 5.f, 1.75f, 20.f, 15.f, 0.0f);

		lines.emplace_back(-300.f, -1.75f, 100.f, -1.75f, 0.2f);
		lines.emplace_back(-300.f, 1.75f, 100.f, 1.75f, 0.2f);

		tp1 = {
			Vec2(-300.f,0.f), Vec2(-299.f,0.f), Vec2(-200.f,0.f),
			Vec2(-150.f,0.f), Vec2(-100.f,0.f), Vec2(-60.f,0.f),
			Vec2(-30.f,0.f), Vec2(0.f,0.f), Vec2(30.f,0.f),
			Vec2(60.f,0.f), Vec2(100.f,0.f), Vec2(100.f, 0.f)
		};
		travels.emplace_back(tp1, 1, 1);
		break;

		// ------------------------------------------------------------------
	case 4: // Simple roundabout
		//mask
		blocks.push_back(Block(-150.0f, -100.0f, 150.0f, 150.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, {}));
		blocks.push_back(Block(-150.0f, 0.0f, -15.0f, 20.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 3, 4, 5, 6 }));
		blocks.push_back(Block(15.0f, -15.0f, 150.0f, 0.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 1, 2, 5, 6 }));
		blocks.push_back(Block(-15.0f, -150.0f, 0.0f, -15.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 1, 2, 3, 4 }));

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
			Vec2(150.f,-1.75f), Vec2(145.f,-1.75f), Vec2(120.f,-1.75f),
			Vec2(100.f,-1.75f), Vec2(90.f,-1.75f), Vec2(60.f,-1.75f),
			Vec2(35.f,-1.75f), Vec2(20.f, -2.5f), Vec2(16.2f,  -6.f),
			Vec2(1.75f,-10.f), Vec2(1.75f, -35.f), Vec2(1.75f, -60.f),
			Vec2(1.75f, -90.f), Vec2(1.75f,-120.f), Vec2(1.75f,-150.f),
			Vec2(1.75f,-150.f)
		};
		// Travel 4: right → left (through roundabout)
		tp4 = {
			Vec2(150.f,-1.75f), Vec2(145.f,-1.75f), Vec2(120.f,-1.75f),
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

		/*conflictPoints.push_back({ Vec2(-13.f,5.f), {6}, {1,2}, 0.f });
		conflictPoints.push_back({ Vec2(13.f,-5.f), {2}, {3,4}, 0.f });
		conflictPoints.push_back({ Vec2(-5.f,-13.f), {4}, {5,6}, 0.f });

		circles.emplace_back(-13.f, 5.f, 2.f, true);
		circles.emplace_back(13.f, -5.f, 2.f, true);
		circles.emplace_back(-5.f, -13.f, 2.f, true);*/

		break;

		// ------------------------------------------------------------------
	case 5: // with traffic lights
		//mask
		blocks.push_back(Block(-300.0f, -300.0f, 300.0f, 300.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, {}));
		blocks.push_back(Block(11.0f, -100.0f, 100.0f, 0.2f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 1, 2, 3, 8, 9, 10 }));
		blocks.push_back(Block(-100.0f, -0.2f, -11.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 4, 5, 6, 7, 12, 13 }));
		blocks.push_back(Block(-11.0f, -100.0f, 11.0f, -3.5f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 4 }));
		blocks.push_back(Block(-3.5f, -10.0f, 11.0f, 0.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 4 }));
		blocks.push_back(Block(-11.0f, 3.5f, 11.0f, 100.f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 8 }));
		blocks.push_back(Block(-11.0f, 0.0f, 3.5f, 10.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 8 }));
		blocks.push_back(Block(-11.0f, -100.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 1, 2, 3, 4, 9, 10, 11, 12, 13, 14 }));
		blocks.push_back(Block(0.0f, 10.0f, 11.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }));

		lightCycle = 50.f; // seconds
		bufforTime = 5.f; // seconds of all-red buffer between light changes
		//lights
		blocks.emplace_back(0.0f, 18.0f, 3.5f, 20.0f,
			0.8f * lightCycle + 4 * bufforTime,
			0.2f * lightCycle,
			0.0f * lightCycle + bufforTime); //down left 4
		blocks.emplace_back(3.5f, 18.0f, 7.0f, 20.0f,
			0.6f * lightCycle + 4 * bufforTime,
			0.4f * lightCycle,
			0.6f * lightCycle + 4 * bufforTime); //down right+up 1
		blocks.emplace_back(7.0f, 18.0f, 10.5f, 20.0f,
			0.6f * lightCycle + 4 * bufforTime,
			0.4f * lightCycle,
			0.6f * lightCycle + 4 * bufforTime); //down right+up 1

		blocks.emplace_back(-3.5f, -20.0f, 0.0f, -18.0f,
			0.8f * lightCycle + 4 * bufforTime,
			0.2f * lightCycle,
			0.0f * lightCycle + bufforTime); //up right 4
		blocks.emplace_back(-7.0f, -20.0f, -3.5f, -18.0f,
			0.6f * lightCycle + 4 * bufforTime,
			0.4f * lightCycle,
			0.6f * lightCycle + 4 * bufforTime); //up left+down 1
		blocks.emplace_back(-10.5f, -20.0f, -7.0f, -18.0f,
			0.6f * lightCycle + 4 * bufforTime,
			0.4f * lightCycle,
			0.6f * lightCycle + 4 * bufforTime); //up left+down 1

		blocks.emplace_back(20.0f, -3.5f, 22.0f, 0.0f,
			0.8f * lightCycle + 4 * bufforTime,
			0.2f * lightCycle,
			0.4f * lightCycle + 3 * bufforTime); //right 2
		blocks.emplace_back(-22.0f, 0.0f, -20.0f, 3.5f,
			0.8f * lightCycle + 4 * bufforTime,
			0.2f * lightCycle,
			0.2f * lightCycle + 2 * bufforTime); //left 3

		lines.emplace_back(0.f, 10.f, 0.f, 250.f, 0.5f);
		lines.emplace_back(-3.5f, 10.f, -3.5f, 250.f, 0.5f);
		lines.emplace_back(0.f, 10.f, -3.5f, 10.f, 0.5f);
		lines.emplace_back(-7.0f, 10.f, -7.0f, 250.f, 0.2f);
		lines.emplace_back(-10.5f, 20.f, -10.5f, 250.f, 0.5f);

		lines.emplace_back(0.f, -10.f, 0.f, -250.f, 0.5f);
		lines.emplace_back(3.5f, -10.f, 3.5f, -250.f, 0.5f);
		lines.emplace_back(0.f, -10.f, 3.5f, -10.f, 0.5f);
		lines.emplace_back(7.0f, -10.f, 7.0f, -250.f, 0.2f);
		lines.emplace_back(10.5f, -20.f, 10.5f, -250.f, 0.5f);

		lines.emplace_back(20.f, -3.5f, 200.f, -3.5f, 0.5f);
		lines.emplace_back(20.f, 0.0f, 200.f, 0.0f, 0.5f);
		lines.emplace_back(20.f, 3.5f, 200.f, 3.5f, 0.5f);

		lines.emplace_back(-20.f, -3.5f, -200.f, -3.5f, 0.5f);
		lines.emplace_back(-20.f, 0.0f, -200.f, 0.0f, 0.5f);
		lines.emplace_back(-20.f, 3.5f, -200.f, 3.5f, 0.5f);

		lines.emplace_back(3.5f, 10.f, 3.5f, 100.f, 0.2f);
		lines.emplace_back(7.0f, 10.f, 7.0f, 100.f, 0.2f);
		lines.emplace_back(10.5f, 20.f, 10.5f, 100.f, 0.5f);
		lines.emplace_back(3.5f, 100.f, 0.0f, 130.f, 0.2f);
		lines.emplace_back(7.0f, 100.f, 3.5f, 130.f, 0.2f);
		lines.emplace_back(10.5f, 100.f, 7.0f, 130.f, 0.5f);
		lines.emplace_back(3.5f, 130.f, 3.5f, 250.f, 0.2f);
		lines.emplace_back(7.0f, 130.f, 7.0f, 250.f, 0.5f);

		lines.emplace_back(-3.5f, -10.f, -3.5f, -100.f, 0.2f);
		lines.emplace_back(-7.0f, -10.f, -7.0f, -100.f, 0.2f);
		lines.emplace_back(-10.5f, -20.f, -10.5f, -100.f, 0.5f);
		lines.emplace_back(-3.5f, -100.f, 0.0f, -130.f, 0.2f);
		lines.emplace_back(-7.0f, -100.f, -3.5f, -130.f, 0.2f);
		lines.emplace_back(-10.5f, -100.f, -7.0f, -130.f, 0.5f);
		lines.emplace_back(-3.5f, -130.f, -3.5f, -250.f, 0.2f);
		lines.emplace_back(-7.0f, -130.f, -7.0f, -250.f, 0.5f);

		lines.emplace_back(10.5f, 20.f, 20.0f, 3.5f, 0.5f);
		lines.emplace_back(-10.5f, -20.f, -20.0f, -3.5f, 0.5f);
		lines.emplace_back(20.0f, -3.5f, 10.5f, -20.0f, 0.5f);
		lines.emplace_back(-20.0f, 3.5f, -10.5f, 20.0f, 0.5f);

		//down right
		tp1 = {
			Vec2(5.25f,250.f), Vec2(5.25f,240.f), Vec2(5.25f,200.f),
			Vec2(5.25f,150.f), Vec2(5.25f,130.f), Vec2(5.25f,122.5f),
			Vec2(7.5f,115.f), Vec2(8.75f,107.5f), Vec2(8.75f,100.f),
			Vec2(8.75f,80.f), Vec2(8.75f,70.f),  Vec2(8.75f,50.f),
			Vec2(8.75f,10.f), Vec2(8.75f,1.75f), Vec2(20.0f,1.75f),
			Vec2(30.0f,1.75f), Vec2(50.0f,1.75f), Vec2(80.0f,1.75f),
			Vec2(100.0f,1.75f), Vec2(150.0f,1.75f), Vec2(200.0f,1.75f)
		};
		//down up 2
		tp2 = {
			Vec2(5.25f,250.f), Vec2(5.25f,240.f), Vec2(5.25f,200.f),
			Vec2(5.25f,150.f), Vec2(5.25f,130.f), Vec2(5.25f,122.5f),
			Vec2(7.5f,115.f), Vec2(8.75f,107.5f), Vec2(8.75f,100.f),
			Vec2(8.75f,80.f), Vec2(8.75f,50.f), Vec2(8.75f,30.f),
			Vec2(8.75f,20.f), Vec2(8.75f,0.f), Vec2(8.75f,-20.f),
			Vec2(8.75f,-50.f), Vec2(8.75f,-100.f), Vec2(8.75f,-150.f),
			Vec2(8.75f,-200.f), Vec2(8.75f,-240.f), Vec2(8.75f,-250.f)
		};
		//down up 1
		tp3 = {
			Vec2(1.75f,250.f), Vec2(1.75f,240.f), Vec2(1.75f,200.f),
			Vec2(1.75f,150.f), Vec2(1.75f,130.f), Vec2(1.75f,122.5f),
			Vec2(4.0f,115.f), Vec2(5.25f,107.5f), Vec2(5.25f,100.f),
			Vec2(5.25f,80.f), Vec2(5.25f,50.f), Vec2(5.25f,30.f),
			Vec2(5.25f,20.f), Vec2(5.25f,0.f), Vec2(5.25f,-20.f),
			Vec2(5.25f,-50.f), Vec2(5.25f,-100.f), Vec2(5.25f,-150.f),
			Vec2(5.25f,-200.f), Vec2(5.25f,-240.f), Vec2(5.25f,-250.f)
		};
		//down left
		tp4 = {
			Vec2(1.75f,250.f), Vec2(1.75f,240.f), Vec2(1.75f,200.f),
			Vec2(1.75f,150.f), Vec2(1.75f,130.f), Vec2(1.75f,100.0f),
			Vec2(1.75f,80.f), Vec2(1.75f,50.f), Vec2(1.75f,12.0f),
			Vec2(1.75f,-1.75f), Vec2(-20.0f,-1.75f), Vec2(-50.0f,-1.75f),
			Vec2(-80.0f,-1.75f), Vec2(-120.0f,-1.75f), Vec2(-150.0f,-1.75f),
			Vec2(-180.0f,-1.75f), Vec2(-200.0f,-1.75f)
		};
		//up left
		tp5 = {
			Vec2(-5.25f,-250.f), Vec2(-5.25f,-240.f), Vec2(-5.25f,-200.f),
			Vec2(-5.25f,-150.f), Vec2(-5.25f,-130.f), Vec2(-5.25f,-122.5f),
			Vec2(-7.5f,-115.f), Vec2(-8.75f,-107.5f), Vec2(-8.75f,-100.f),
			Vec2(-8.75f,-80.f), Vec2(-8.75f,-70.f), Vec2(-8.75f,-50.f),
			Vec2(-8.75f,-10.f), Vec2(-8.75f,-1.75f), Vec2(-20.0f,-1.75f),
			Vec2(-30.0f,-1.75f), Vec2(-50.0f,-1.75f), Vec2(-80.0f,-1.75f),
			Vec2(-100.0f,-1.75f), Vec2(-150.0f,-1.75f), Vec2(-200.0f,-1.75f)
		};
		//up down 2
		tp6 = {
			Vec2(-5.25f,-250.f), Vec2(-5.25f,-240.f), Vec2(-5.25f,-200.f),
			Vec2(-5.25f,-150.f), Vec2(-5.25f,-130.f), Vec2(-5.25f,-122.5f),
			Vec2(-7.5f,-115.f), Vec2(-8.75f,-107.5f), Vec2(-8.75f,-100.f),
			Vec2(-8.75f,-80.f), Vec2(-8.75f,-50.f), Vec2(-8.75f,-30.f),
			Vec2(-8.75f,-20.f), Vec2(-8.75f,-0.f), Vec2(-8.75f,20.f),
			Vec2(-8.75f,50.f), Vec2(-8.75f,100.f), Vec2(-8.75f,150.f),
			Vec2(-8.75f,200.f), Vec2(-8.75f,240.f), Vec2(-8.75f,250.f)
		};
		//up down 1
		tp7 = {
			Vec2(-1.75f,-250.f), Vec2(-1.75f,-240.f), Vec2(-1.75f,-200.f),
			Vec2(-1.75f,-150.f), Vec2(-1.75f,-130.f), Vec2(-1.75f,-122.5f),
			Vec2(-4.0f,-115.f), Vec2(-5.25f,-107.5f), Vec2(-5.25f,-100.f),
			Vec2(-5.25f,-80.f), Vec2(-5.25f,-50.f), Vec2(-5.25f,-30.f),
			Vec2(-5.25f,-20.f), Vec2(-5.25f,-0.f), Vec2(-5.25f,20.f),
			Vec2(-5.25f,50.f), Vec2(-5.25f,100.f), Vec2(-5.25f,150.f),
			Vec2(-5.25f,200.f), Vec2(-5.25f,240.f), Vec2(-5.25f,250.f)
		};
		//up right
		tp8 = {
			Vec2(-1.75f,-250.f), Vec2(-1.75f,-240.f), Vec2(-1.75f,-200.f),
			Vec2(-1.75f,-150.f), Vec2(-1.75f,-130.f), Vec2(-1.75f,-100.0f),
			Vec2(-1.75f,-80.f), Vec2(-1.75f,-50.f), Vec2(-1.75f,-12.0f),
			Vec2(-1.75f,1.75f), Vec2(20.0f,1.75f), Vec2(50.0f,1.75f),
			Vec2(80.0f,1.75f), Vec2(120.0f,1.75f), Vec2(150.0f,1.75f),
			Vec2(180.0f,1.75f), Vec2(200.0f,1.75f)
		};
		//left right
		tp9 = {
			Vec2(-200.0f,1.75f), Vec2(-180.0f,1.75f), Vec2(-150.0f,1.75f),
			Vec2(-120.0f,1.75f), Vec2(-80.0f,1.75f), Vec2(-50.0f,1.75f),
			Vec2(-20.0f,1.75f), Vec2(-1.75f,1.75f), Vec2(1.75f,1.75f),
			Vec2(20.0f,1.75f), Vec2(50.0f,1.75f), Vec2(80.0f,1.75f),
			Vec2(120.0f,1.75f), Vec2(150.0f,1.75f), Vec2(180.0f,1.75f),
			Vec2(200.0f,1.75f)
		};
		//left up
		tp10 = {
			Vec2(-200.0f,1.75f), Vec2(-180.0f,1.75f), Vec2(-150.0f,1.75f),
			Vec2(-120.0f,1.75f), Vec2(-80.0f,1.75f), Vec2(-50.0f,1.75f),
			Vec2(-20.0f,1.75f), Vec2(-10.0f,1.75f), Vec2(-5.0f,1.75f),
			Vec2(8.75f,1.75f), Vec2(8.75f,-10.0f), Vec2(8.75f,-30.0f),
			Vec2(8.75f,-80.0f), Vec2(8.75f,-120.0f), Vec2(8.75f,-180.0f),
			Vec2(8.75f,-230.0f), Vec2(8.75f,-250.0f)
		};
		//left down
		tp11 = {
			Vec2(-200.0f,1.75f), Vec2(-180.0f,1.75f), Vec2(-150.0f,1.75f),
			Vec2(-120.0f,1.75f), Vec2(-80.0f,1.75f), Vec2(-50.0f,1.75f),
			Vec2(-20.0f,1.75f), Vec2(-8.75f,1.75f), Vec2(-8.75f,20.0f),
			Vec2(-8.75f,50.0f), Vec2(-8.75f,100.0f), Vec2(-8.75f,150.0f),
			Vec2(-8.75f,200.0f), Vec2(-8.75f,230.0f), Vec2(-8.75f,250.0f)
		};
		//right left
		tp12 = {
			Vec2(200.0f,-1.75f), Vec2(180.0f,-1.75f), Vec2(150.0f,-1.75f),
			Vec2(120.0f,-1.75f), Vec2(80.0f,-1.75f), Vec2(50.0f,-1.75f),
			Vec2(20.0f,-1.75f), Vec2(1.75f,-1.75f), Vec2(-1.75f,-1.75f),
			Vec2(-20.0f,-1.75f), Vec2(-50.0f,-1.75f), Vec2(-80.0f,-1.75f),
			Vec2(-120.0f,-1.75f), Vec2(-150.0f,-1.75f), Vec2(-180.0f,-1.75f),
			Vec2(-200.0f,-1.75f)
		};
		//right down
		tp13 = {
			Vec2(200.0f,-1.75f), Vec2(180.0f,-1.75f), Vec2(150.0f,-1.75f),
			Vec2(120.0f,-1.75f), Vec2(80.0f,-1.75f), Vec2(50.0f,-1.75f),
			Vec2(20.0f,-1.75f), Vec2(10.0f,-1.75f), Vec2(5.0f,-1.75f),
			Vec2(-8.75f,-1.75f), Vec2(-8.75f,10.0f), Vec2(-8.75f,30.0f),
			Vec2(-8.75f,80.0f), Vec2(-8.75f,120.0f), Vec2(-8.75f,180.0f),
			Vec2(-8.75f,230.0f), Vec2(-8.75f,250.0f)
		};
		//right up
		tp14 = {
			Vec2(200.0f,-1.75f), Vec2(180.0f,-1.75f), Vec2(150.0f,-1.75f),
			Vec2(120.0f,-1.75f), Vec2(80.0f,-1.75f), Vec2(50.0f,-1.75f),
			Vec2(20.0f,-1.75f), Vec2(8.75f,-1.75f), Vec2(8.75f,-20.0f),
			Vec2(8.75f,-50.0f), Vec2(8.75f,-100.0f), Vec2(8.75f,-150.0f),
			Vec2(8.75f,-200.0f), Vec2(8.75f,-230.0f), Vec2(8.75f,-250.0f)
		};

		travels.emplace_back(tp1, 3, 1);
		travels.emplace_back(tp2, 6, 2);
		travels.emplace_back(tp3, 6, 3);
		travels.emplace_back(tp4, 3, 4);
		travels.emplace_back(tp5, 3, 5);
		travels.emplace_back(tp6, 6, 6);
		travels.emplace_back(tp7, 6, 7);
		travels.emplace_back(tp8, 3, 8);
		travels.emplace_back(tp9, 2, 9);
		travels.emplace_back(tp10, 2, 10);
		travels.emplace_back(tp11, 2, 11);
		travels.emplace_back(tp12, 2, 12);
		travels.emplace_back(tp13, 2, 13);
		travels.emplace_back(tp14, 2, 14);

		//circles.append_range(drawTravel(tp1));
		break;
	case 6:
		//mask
		blocks.push_back(Block(-300.0f, -300.0f, 300.0f, 300.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, {}));
		blocks.push_back(Block(11.0f, -100.0f, 100.0f, 0.2f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 1, 2, 3, 8, 9, 10 }));
		blocks.push_back(Block(-100.0f, -0.2f, -11.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 4, 5, 6, 7, 12, 13 }));
		blocks.push_back(Block(-11.0f, -100.0f, 11.0f, -3.5f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 4 }));
		blocks.push_back(Block(-3.5f, -10.0f, 11.0f, 0.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 4 }));
		blocks.push_back(Block(-11.0f, 3.5f, 11.0f, 100.f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 8 }));
		blocks.push_back(Block(-11.0f, 0.0f, 3.5f, 10.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 8 }));
		blocks.push_back(Block(-11.0f, -100.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 1, 2, 3, 4, 9, 10, 11, 12, 13, 14 }));
		blocks.push_back(Block(0.0f, 10.0f, 11.0f, 100.0f, 0.0f, 1.0f, 0.0f, BlockType::PerceptionMask, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }));

		lightCycle = 50.f; // seconds
		bufforTime = 5.f; // seconds of all-red buffer between light changes
		//lights
		blocks.emplace_back(0.0f, 18.0f, 3.5f, 20.0f,
			0.8f * lightCycle + 4 * bufforTime,
			0.2f * lightCycle,
			0.0f * lightCycle + bufforTime); //down left 4
		blocks.emplace_back(3.5f, 18.0f, 7.0f, 20.0f,
			0.6f * lightCycle + 4 * bufforTime,
			0.4f * lightCycle,
			0.6f * lightCycle + 4 * bufforTime); //down right+up 1
		blocks.emplace_back(7.0f, 18.0f, 10.5f, 20.0f,
			0.6f * lightCycle + 4 * bufforTime,
			0.4f * lightCycle,
			0.6f * lightCycle + 4 * bufforTime); //down right+up 1

		blocks.emplace_back(-3.5f, -20.0f, 0.0f, -18.0f,
			0.8f * lightCycle + 4 * bufforTime,
			0.2f * lightCycle,
			0.0f * lightCycle + bufforTime); //up right 4
		blocks.emplace_back(-7.0f, -20.0f, -3.5f, -18.0f,
			0.6f * lightCycle + 4 * bufforTime,
			0.4f * lightCycle,
			0.6f * lightCycle + 4 * bufforTime); //up left+down 1
		blocks.emplace_back(-10.5f, -20.0f, -7.0f, -18.0f,
			0.6f * lightCycle + 4 * bufforTime,
			0.4f * lightCycle,
			0.6f * lightCycle + 4 * bufforTime); //up left+down 1

		blocks.emplace_back(20.0f, -3.5f, 22.0f, 0.0f,
			0.8f * lightCycle + 4 * bufforTime,
			0.2f * lightCycle,
			0.4f * lightCycle + 3 * bufforTime); //right 2
		blocks.emplace_back(-22.0f, 0.0f, -20.0f, 3.5f,
			0.8f * lightCycle + 4 * bufforTime,
			0.2f * lightCycle,
			0.2f * lightCycle + 2 * bufforTime); //left 3

		lines.emplace_back(0.f, 10.f, 0.f, 250.f, 0.5f);
		lines.emplace_back(-3.5f, 10.f, -3.5f, 250.f, 0.5f);
		lines.emplace_back(0.f, 10.f, -3.5f, 10.f, 0.5f);
		lines.emplace_back(-7.0f, 10.f, -7.0f, 250.f, 0.2f);
		lines.emplace_back(-10.5f, 20.f, -10.5f, 250.f, 0.5f);

		lines.emplace_back(0.f, -10.f, 0.f, -250.f, 0.5f);
		lines.emplace_back(3.5f, -10.f, 3.5f, -250.f, 0.5f);
		lines.emplace_back(0.f, -10.f, 3.5f, -10.f, 0.5f);
		lines.emplace_back(7.0f, -10.f, 7.0f, -250.f, 0.2f);
		lines.emplace_back(10.5f, -20.f, 10.5f, -250.f, 0.5f);

		lines.emplace_back(20.f, -3.5f, 200.f, -3.5f, 0.5f);
		lines.emplace_back(20.f, 0.0f, 200.f, 0.0f, 0.5f);
		lines.emplace_back(20.f, 3.5f, 200.f, 3.5f, 0.5f);

		lines.emplace_back(-20.f, -3.5f, -200.f, -3.5f, 0.5f);
		lines.emplace_back(-20.f, 0.0f, -200.f, 0.0f, 0.5f);
		lines.emplace_back(-20.f, 3.5f, -200.f, 3.5f, 0.5f);

		lines.emplace_back(3.5f, 10.f, 3.5f, 100.f, 0.2f);
		lines.emplace_back(7.0f, 10.f, 7.0f, 100.f, 0.2f);
		lines.emplace_back(10.5f, 20.f, 10.5f, 100.f, 0.5f);
		lines.emplace_back(3.5f, 100.f, 0.0f, 130.f, 0.2f);
		lines.emplace_back(7.0f, 100.f, 3.5f, 130.f, 0.2f);
		lines.emplace_back(10.5f, 100.f, 7.0f, 130.f, 0.5f);
		lines.emplace_back(3.5f, 130.f, 3.5f, 250.f, 0.2f);
		lines.emplace_back(7.0f, 130.f, 7.0f, 250.f, 0.5f);

		lines.emplace_back(-3.5f, -10.f, -3.5f, -100.f, 0.2f);
		lines.emplace_back(-7.0f, -10.f, -7.0f, -100.f, 0.2f);
		lines.emplace_back(-10.5f, -20.f, -10.5f, -100.f, 0.5f);
		lines.emplace_back(-3.5f, -100.f, 0.0f, -130.f, 0.2f);
		lines.emplace_back(-7.0f, -100.f, -3.5f, -130.f, 0.2f);
		lines.emplace_back(-10.5f, -100.f, -7.0f, -130.f, 0.5f);
		lines.emplace_back(-3.5f, -130.f, -3.5f, -250.f, 0.2f);
		lines.emplace_back(-7.0f, -130.f, -7.0f, -250.f, 0.5f);

		lines.emplace_back(10.5f, 20.f, 20.0f, 3.5f, 0.5f);
		lines.emplace_back(-10.5f, -20.f, -20.0f, -3.5f, 0.5f);
		lines.emplace_back(20.0f, -3.5f, 10.5f, -20.0f, 0.5f);
		lines.emplace_back(-20.0f, 3.5f, -10.5f, 20.0f, 0.5f);

		//down right
		tp1 = {
			Vec2(5.25f,250.f), Vec2(5.25f,240.f), Vec2(5.25f,200.f),
			Vec2(5.25f,150.f), Vec2(5.25f,130.f), Vec2(5.25f,122.5f),
			Vec2(7.5f,115.f), Vec2(8.75f,107.5f), Vec2(8.75f,100.f),
			Vec2(8.75f,80.f), Vec2(8.75f,70.f),  Vec2(8.75f,50.f),
			Vec2(8.75f,10.f), Vec2(8.75f,1.75f), Vec2(20.0f,1.75f),
			Vec2(30.0f,1.75f), Vec2(50.0f,1.75f), Vec2(80.0f,1.75f),
			Vec2(100.0f,1.75f), Vec2(150.0f,1.75f), Vec2(200.0f,1.75f)
		};
		//down up 2
		tp2 = {
			Vec2(5.25f,250.f), Vec2(5.25f,240.f), Vec2(5.25f,200.f),
			Vec2(5.25f,150.f), Vec2(5.25f,130.f), Vec2(5.25f,122.5f),
			Vec2(7.5f,115.f), Vec2(8.75f,107.5f), Vec2(8.75f,100.f),
			Vec2(8.75f,80.f), Vec2(8.75f,50.f), Vec2(8.75f,30.f),
			Vec2(8.75f,20.f), Vec2(8.75f,0.f), Vec2(8.75f,-20.f),
			Vec2(8.75f,-50.f), Vec2(8.75f,-100.f), Vec2(8.75f,-150.f),
			Vec2(8.75f,-200.f), Vec2(8.75f,-240.f), Vec2(8.75f,-250.f)
		};
		//down up 1
		tp3 = {
			Vec2(1.75f,250.f), Vec2(1.75f,240.f), Vec2(1.75f,200.f),
			Vec2(1.75f,150.f), Vec2(1.75f,130.f), Vec2(1.75f,122.5f),
			Vec2(4.0f,115.f), Vec2(5.25f,107.5f), Vec2(5.25f,100.f),
			Vec2(5.25f,80.f), Vec2(5.25f,50.f), Vec2(5.25f,30.f),
			Vec2(5.25f,20.f), Vec2(5.25f,0.f), Vec2(5.25f,-20.f),
			Vec2(5.25f,-50.f), Vec2(5.25f,-100.f), Vec2(5.25f,-150.f),
			Vec2(5.25f,-200.f), Vec2(5.25f,-240.f), Vec2(5.25f,-250.f)
		};
		//down left
		tp4 = {
			Vec2(1.75f,250.f), Vec2(1.75f,240.f), Vec2(1.75f,200.f),
			Vec2(1.75f,150.f), Vec2(1.75f,130.f), Vec2(1.75f,100.0f),
			Vec2(1.75f,80.f), Vec2(1.75f,50.f), Vec2(1.75f,12.0f),
			Vec2(1.75f,-1.75f), Vec2(-20.0f,-1.75f), Vec2(-50.0f,-1.75f),
			Vec2(-80.0f,-1.75f), Vec2(-120.0f,-1.75f), Vec2(-150.0f,-1.75f),
			Vec2(-180.0f,-1.75f), Vec2(-200.0f,-1.75f)
		};
		//up left
		tp5 = {
			Vec2(-5.25f,-250.f), Vec2(-5.25f,-240.f), Vec2(-5.25f,-200.f),
			Vec2(-5.25f,-150.f), Vec2(-5.25f,-130.f), Vec2(-5.25f,-122.5f),
			Vec2(-7.5f,-115.f), Vec2(-8.75f,-107.5f), Vec2(-8.75f,-100.f),
			Vec2(-8.75f,-80.f), Vec2(-8.75f,-70.f), Vec2(-8.75f,-50.f),
			Vec2(-8.75f,-10.f), Vec2(-8.75f,-1.75f), Vec2(-20.0f,-1.75f),
			Vec2(-30.0f,-1.75f), Vec2(-50.0f,-1.75f), Vec2(-80.0f,-1.75f),
			Vec2(-100.0f,-1.75f), Vec2(-150.0f,-1.75f), Vec2(-200.0f,-1.75f)
		};
		//up down 2
		tp6 = {
			Vec2(-5.25f,-250.f), Vec2(-5.25f,-240.f), Vec2(-5.25f,-200.f),
			Vec2(-5.25f,-150.f), Vec2(-5.25f,-130.f), Vec2(-5.25f,-122.5f),
			Vec2(-7.5f,-115.f), Vec2(-8.75f,-107.5f), Vec2(-8.75f,-100.f),
			Vec2(-8.75f,-80.f), Vec2(-8.75f,-50.f), Vec2(-8.75f,-30.f),
			Vec2(-8.75f,-20.f), Vec2(-8.75f,-0.f), Vec2(-8.75f,20.f),
			Vec2(-8.75f,50.f), Vec2(-8.75f,100.f), Vec2(-8.75f,150.f),
			Vec2(-8.75f,200.f), Vec2(-8.75f,240.f), Vec2(-8.75f,250.f)
		};
		//up down 1
		tp7 = {
			Vec2(-1.75f,-250.f), Vec2(-1.75f,-240.f), Vec2(-1.75f,-200.f),
			Vec2(-1.75f,-150.f), Vec2(-1.75f,-130.f), Vec2(-1.75f,-122.5f),
			Vec2(-4.0f,-115.f), Vec2(-5.25f,-107.5f), Vec2(-5.25f,-100.f),
			Vec2(-5.25f,-80.f), Vec2(-5.25f,-50.f), Vec2(-5.25f,-30.f),
			Vec2(-5.25f,-20.f), Vec2(-5.25f,-0.f), Vec2(-5.25f,20.f),
			Vec2(-5.25f,50.f), Vec2(-5.25f,100.f), Vec2(-5.25f,150.f),
			Vec2(-5.25f,200.f), Vec2(-5.25f,240.f), Vec2(-5.25f,250.f)
		};
		//up right
		tp8 = {
			Vec2(-1.75f,-250.f), Vec2(-1.75f,-240.f), Vec2(-1.75f,-200.f),
			Vec2(-1.75f,-150.f), Vec2(-1.75f,-130.f), Vec2(-1.75f,-100.0f),
			Vec2(-1.75f,-80.f), Vec2(-1.75f,-50.f), Vec2(-1.75f,-12.0f),
			Vec2(-1.75f,1.75f), Vec2(20.0f,1.75f), Vec2(50.0f,1.75f),
			Vec2(80.0f,1.75f), Vec2(120.0f,1.75f), Vec2(150.0f,1.75f),
			Vec2(180.0f,1.75f), Vec2(200.0f,1.75f)
		};
		//left right
		tp9 = {
			Vec2(-200.0f,1.75f), Vec2(-180.0f,1.75f), Vec2(-150.0f,1.75f),
			Vec2(-120.0f,1.75f), Vec2(-80.0f,1.75f), Vec2(-50.0f,1.75f),
			Vec2(-20.0f,1.75f), Vec2(-1.75f,1.75f), Vec2(1.75f,1.75f),
			Vec2(20.0f,1.75f), Vec2(50.0f,1.75f), Vec2(80.0f,1.75f),
			Vec2(120.0f,1.75f), Vec2(150.0f,1.75f), Vec2(180.0f,1.75f),
			Vec2(200.0f,1.75f)
		};
		//left up
		tp10 = {
			Vec2(-200.0f,1.75f), Vec2(-180.0f,1.75f), Vec2(-150.0f,1.75f),
			Vec2(-120.0f,1.75f), Vec2(-80.0f,1.75f), Vec2(-50.0f,1.75f),
			Vec2(-20.0f,1.75f), Vec2(-10.0f,1.75f), Vec2(-5.0f,1.75f),
			Vec2(8.75f,1.75f), Vec2(8.75f,-10.0f), Vec2(8.75f,-30.0f),
			Vec2(8.75f,-80.0f), Vec2(8.75f,-120.0f), Vec2(8.75f,-180.0f),
			Vec2(8.75f,-230.0f), Vec2(8.75f,-250.0f)
		};
		//left down
		tp11 = {
			Vec2(-200.0f,1.75f), Vec2(-180.0f,1.75f), Vec2(-150.0f,1.75f),
			Vec2(-120.0f,1.75f), Vec2(-80.0f,1.75f), Vec2(-50.0f,1.75f),
			Vec2(-20.0f,1.75f), Vec2(-8.75f,1.75f), Vec2(-8.75f,20.0f),
			Vec2(-8.75f,50.0f), Vec2(-8.75f,100.0f), Vec2(-8.75f,150.0f),
			Vec2(-8.75f,200.0f), Vec2(-8.75f,230.0f), Vec2(-8.75f,250.0f)
		};
		//right left
		tp12 = {
			Vec2(200.0f,-1.75f), Vec2(180.0f,-1.75f), Vec2(150.0f,-1.75f),
			Vec2(120.0f,-1.75f), Vec2(80.0f,-1.75f), Vec2(50.0f,-1.75f),
			Vec2(20.0f,-1.75f), Vec2(1.75f,-1.75f), Vec2(-1.75f,-1.75f),
			Vec2(-20.0f,-1.75f), Vec2(-50.0f,-1.75f), Vec2(-80.0f,-1.75f),
			Vec2(-120.0f,-1.75f), Vec2(-150.0f,-1.75f), Vec2(-180.0f,-1.75f),
			Vec2(-200.0f,-1.75f)
		};
		//right down
		tp13 = {
			Vec2(200.0f,-1.75f), Vec2(180.0f,-1.75f), Vec2(150.0f,-1.75f),
			Vec2(120.0f,-1.75f), Vec2(80.0f,-1.75f), Vec2(50.0f,-1.75f),
			Vec2(20.0f,-1.75f), Vec2(10.0f,-1.75f), Vec2(5.0f,-1.75f),
			Vec2(-8.75f,-1.75f), Vec2(-8.75f,10.0f), Vec2(-8.75f,30.0f),
			Vec2(-8.75f,80.0f), Vec2(-8.75f,120.0f), Vec2(-8.75f,180.0f),
			Vec2(-8.75f,230.0f), Vec2(-8.75f,250.0f)
		};
		//right up
		tp14 = {
			Vec2(200.0f,-1.75f), Vec2(180.0f,-1.75f), Vec2(150.0f,-1.75f),
			Vec2(120.0f,-1.75f), Vec2(80.0f,-1.75f), Vec2(50.0f,-1.75f),
			Vec2(20.0f,-1.75f), Vec2(8.75f,-1.75f), Vec2(8.75f,-20.0f),
			Vec2(8.75f,-50.0f), Vec2(8.75f,-100.0f), Vec2(8.75f,-150.0f),
			Vec2(8.75f,-200.0f), Vec2(8.75f,-230.0f), Vec2(8.75f,-250.0f)
		};

		travels.emplace_back(tp1, 3, 1);
		travels.emplace_back(tp2, 6, 2);
		travels.emplace_back(tp3, 6, 3);
		travels.emplace_back(tp4, 3, 4);
		travels.emplace_back(tp5, 3, 5);
		travels.emplace_back(tp6, 6, 6);
		travels.emplace_back(tp7, 6, 7);
		travels.emplace_back(tp8, 3, 8);
		travels.emplace_back(tp9, 2, 9);
		travels.emplace_back(tp10, 2, 10);
		travels.emplace_back(tp11, 2, 11);
		travels.emplace_back(tp12, 2, 12);
		travels.emplace_back(tp13, 2, 13);
		travels.emplace_back(tp14, 2, 14);
	default:
		std::cerr << "Junction: unknown index " << index << "\n";
		break;
	}
}

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

	int rnd = rand() % total;
	for (const auto& tr : travels)
	{
		if (rnd < tr.getWeight()) return tr;
		rnd -= tr.getWeight();
	}

	return travels.back();
}
std::vector<Circle> Junction::drawTravel(std::vector<Vec2> travelPoints) {
	Travel tempTravel(travelPoints, 1, 0);
	const float STEP = 0.05f;
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

const Block* Junction::getPerceptionMaskForTravel(int travelId, const Vec2& point) const
{
	for (const auto& block : blocks)
	{
		if (block.getType() != BlockType::PerceptionMask)
			continue;

		if (!block.masksTravel(travelId))
			continue;

		if (!block.containsPoint(point))
			continue;

		return &block;
	}

	return nullptr;
}

const std::vector<ConflictPoint>& Junction::getConflictPoints() const { return conflictPoints; }