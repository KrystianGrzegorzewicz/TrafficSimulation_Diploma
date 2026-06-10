#include "vehicles/VehicleFactory.h"
#include "vehicles/CarHuman.h"
#include "vehicles/CarAV.h"
#include "behavior/BehaviorHuman.h"
#include "behavior/longitudinal/IDMLongitudinalModel.h"
#include "behavior/longitudinal/FuzzyLongitudinalModel.h"
#include "behavior/BehaviorAV.h"
#include "perception/PerceptionHuman.h"
#include "perception/PerceptionAV.h"
#include "vehicles/DriverPersonalityGenerator.h"

#include <memory>

std::unique_ptr<Car> VehicleFactory::createHuman(float speed, Travel travel)
{
	DriverPersonality personality = DriverPersonalityGenerator::generate();

	return std::make_unique<CarHuman>(
		speed,
		std::move(travel),
		personality,
		std::make_unique<BehaviorHuman>(
			std::make_unique<IDMLongitudinalModel>(),
			personality
		),
		std::make_unique<PerceptionHuman>()
	);
}

std::unique_ptr<Car> VehicleFactory::createAV(float speed, Travel travel)
{
	return std::make_unique<CarAV>(
		speed,
		std::move(travel),
		std::make_unique<BehaviorAV>(
			std::make_unique<FuzzyLongitudinalModel>()
		),
		std::make_unique<PerceptionAV>()
	);
}