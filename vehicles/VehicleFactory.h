#pragma once

#include "vehicles/Car.h"
#include "road/Travel.h"

#include <memory>

class VehicleFactory
{
public:
	static std::unique_ptr<Car> createHuman(float  initialSpeed, Travel travel);
	static std::unique_ptr<Car> createAV(float  initialSpeed, Travel travel, const std::string& avModel);
};