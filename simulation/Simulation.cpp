#include "simulation/Simulation.h"
#include "vehicles/VehicleFactory.h"
#include "vehicles/DriverPersonality.h"

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <filesystem>

static std::string getTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto t = std::chrono::system_clock::to_time_t(now);

	std::tm tm{};
#ifdef _WIN32
	localtime_s(&tm, &t);
#else
	localtime_r(&t, &tm);
#endif

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
	return oss.str();
}

Simulation::Simulation(
	float spawnRate,
	int junctionIndex,
	bool saveCsv,
	float AVrate,
	const Config& conf)
	: junction(junctionIndex)
	, saveCsv(saveCsv)
	, AVrate(AVrate)
{
	spawnPeriod =
		(spawnRate > 0.f)
		? (1.f / spawnRate)
		: 1e9f;

	worldState.junction = &junction;

	if (saveCsv)
	{
		std::ostringstream experimentFolder;

		experimentFolder
			<< "data/";

		if (!conf.experimentName.empty())
		{
			experimentFolder
				<< conf.experimentName;
		}
		else
		{
			experimentFolder
				<< "J" << conf.junction
				<< "_AV" << static_cast<int>(conf.AVRate * 100)
				<< "_SP" << static_cast<int>(conf.spawnRate * 100)
				<< "_BIAS" << static_cast<int>(conf.aggressionBias * 100)
				<< "_SHARP" << static_cast<int>(conf.aggressionSharpness * 100)
				<< "_SEED" << conf.randomSeed;
		}

		outputDirectory = experimentFolder.str();

		std::filesystem::create_directories(
			outputDirectory);

		std::string carsFilename =
			outputDirectory +
			"/cars_run" +
			std::to_string(conf.runId) +
			".csv";

		std::string personalityFilename =
			outputDirectory +
			"/personality_run" +
			std::to_string(conf.runId) +
			".csv";

		logFile.open(carsFilename);

		personalityFile.open(
			personalityFilename);

		personalityFile
			<< "id,"
			<< "aggression,"
			<< "accel_factor,"
			<< "gap_factor,"
			<< "reaction_factor,"
			<< "start_delay,"
			<< "curve_factor\n";

		logFile
			<< "time,"
			<< "id,"
			<< "travel_id,"
			<< "x,"
			<< "y,"
			<< "vx,"
			<< "vy,"
			<< "ax,"
			<< "ay\n";
	}

	spawnVehicle();
}

Simulation::~Simulation()
{
	if (logFile.is_open())
		logFile.close();

	if (personalityFile.is_open())
		personalityFile.close();
}

void Simulation::step(float dt)
{
	currentTime += dt;
	worldState.currentTime = currentTime;

	// Spawn new vehicles on schedule
	spawnAccumulator += dt;
	while (spawnAccumulator >= spawnPeriod)
	{
		spawnVehicle();
		spawnAccumulator -= spawnPeriod;
	}

	// Tick block on/off cycles
	for (auto& block : junction.getBlocks())
		block.update(dt);

	// Snapshot all vehicle states ONCE before any vehicle updates,
	// so every vehicle reads consistent state from the same instant.
	rebuildVehicleStates();

	// Update all vehicles — Simulation is fully agnostic to Human vs AV
	for (auto& v : vehicles)
		v->update(dt, worldState);

	pruneFinished();

	if (saveCsv)
		sendCsv();
}

void Simulation::spawnVehicle()
{
	std::unique_ptr<Car> vehicle;

	if (AVrate > 0.f && (rand() / (float)RAND_MAX) < AVrate)
	{
		vehicle = VehicleFactory::createAV(14.f, junction.getRandomTravel());
	}
	else
	{
		vehicle = VehicleFactory::createHuman(14.f, junction.getRandomTravel());
	}

	if (saveCsv)
	{
		const DriverPersonality* personality =
			vehicle->getPersonality();

		if (personality != nullptr)
		{
			personalityFile
				<< vehicle->getId() << ","
				<< personality->aggression << ","
				<< personality->accelFactor << ","
				<< personality->gapFactor << ","
				<< personality->reactionFactor << ","
				<< personality->startDelay << ","
				<< personality->curveFactor
				<< "\n";
		}
	}

	vehicles.push_back(std::move(vehicle));
}

void Simulation::rebuildVehicleStates()
{
	worldState.vehicleStates.clear();
	worldState.vehicleStates.reserve(vehicles.size());

	for (const auto& v : vehicles)
		worldState.vehicleStates.push_back(v->getState());
}

void Simulation::pruneFinished()
{
	auto it =
		std::remove_if(
			vehicles.begin(),
			vehicles.end(),
			[this](const std::unique_ptr<Car>& c)
			{
				if (c->isFinished())
				{
					finishedVehicles++;
					return true;
				}

				return false;
			});

	vehicles.erase(it, vehicles.end());
}

void Simulation::sendCsv()
{
	for (const auto& v : vehicles)
	{
		Vec2 pos = v->getPosition();
		Vec2 vel = v->getVelocityVector();
		Vec2 acc = v->getAccelerationVector();

		logFile << currentTime << ","
			<< v->getId() << ","
			<< v->getTravelId() << ","
			<< pos.x << "," << pos.y << ","
			<< vel.x << "," << vel.y << ","
			<< acc.x << "," << acc.y << "\n";
	}
}

std::string Simulation::getWorldJson()
{
	std::ostringstream ss;
	ss << "{";

	ss << "\"cars\":[";
	for (std::size_t i = 0; i < vehicles.size(); ++i)
	{
		Vec2 pos = vehicles[i]->getPosition();
		Vec2 vel = vehicles[i]->getVelocityVector();
		Vec2 acc = vehicles[i]->getAccelerationVector();
		int* color = vehicles[i]->getColor();

		ss << "{\"x\":" << pos.x
			<< ",\"y\":" << pos.y
			<< ",\"vx\":" << vel.x
			<< ",\"vy\":" << vel.y
			<< ",\"ax\":" << acc.x
			<< ",\"ay\":" << acc.y
			<< ",\"color_r\":" << color[0]
			<< ",\"color_g\":" << color[1]
			<< ",\"color_b\":" << color[2]
			<< "}";

		if (i + 1 < vehicles.size()) ss << ",";
	}
	ss << "]";

	ss << ",\"blocks\":[";
	const auto& blocks = junction.getBlocks();
	for (std::size_t i = 0; i < blocks.size(); ++i)
	{
		Vec2 tl = blocks[i].getTopLeft();
		Vec2 br = blocks[i].getBottomRight();
		bool active = blocks[i].isActive();
		bool isMask = (blocks[i].getType() == BlockType::PerceptionMask);

		ss << "{\"x1\":" << tl.x
			<< ",\"y1\":" << tl.y
			<< ",\"x2\":" << br.x
			<< ",\"y2\":" << br.y
			<< ",\"active\":" << (active ? 1 : 0)
			<< ",\"isMask\":" << (isMask ? 1 : 0) << "}";

		if (i + 1 < blocks.size()) ss << ",";
	}
	ss << "]";

	ss << ",\"lines\":[";
	auto lines = junction.getLines();
	for (std::size_t i = 0; i < lines.size(); ++i)
	{
		Vec2  start = lines[i].getStart();
		Vec2  end = lines[i].getEnd();
		float thickness = lines[i].getThickness();

		ss << "{\"x1\":" << start.x
			<< ",\"y1\":" << start.y
			<< ",\"x2\":" << end.x
			<< ",\"y2\":" << end.y
			<< ",\"thickness\":" << thickness
			<< ",\"r\":" << lines[i].getColor(0)
			<< ",\"g\":" << lines[i].getColor(1)
			<< ",\"b\":" << lines[i].getColor(2) << "}";

		if (i + 1 < lines.size()) ss << ",";
	}
	ss << "]";

	ss << ",\"circles\":[";
	auto circles = junction.getCircles();
	for (std::size_t i = 0; i < circles.size(); ++i)
	{
		Vec2  center = circles[i].getCenter();
		float radius = circles[i].getRadius();
		bool  filled = circles[i].isFilled();

		ss << "{\"cx\":" << center.x
			<< ",\"cy\":" << center.y
			<< ",\"radius\":" << radius
			<< ",\"filled\":" << (filled ? "true" : "false")
			<< ",\"r\":" << circles[i].getColor(0)
			<< ",\"g\":" << circles[i].getColor(1)
			<< ",\"b\":" << circles[i].getColor(2) << "}";

		if (i + 1 < circles.size()) ss << ",";
	}
	ss << "]";

	ss << "}\n";
	return ss.str();
}