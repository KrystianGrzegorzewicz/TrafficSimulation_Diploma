#include "core/Config.h"

#include <string>
#include <cstdlib>

Config parseConfig(int argc, char* argv[])
{
	Config conf;

	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];

		if (arg == "--nogui")
			conf.gui = false;

		else if (arg == "--no-csv")
			conf.saveCsv = false;

		else if (arg == "--spawn")
			conf.spawnRate = std::stof(argv[++i]);

		else if (arg == "--av")
			conf.AVRate = std::stof(argv[++i]);

		else if (arg == "--avmodel")
			conf.AVModel = argv[++i];

		else if (arg == "--junction")
			conf.junction = std::stoi(argv[++i]);

		else if (arg == "--speed")
			conf.simSpeed = std::stof(argv[++i]);

		else if (arg == "--bias")
			conf.aggressionBias = std::stof(argv[++i]);

		else if (arg == "--sharpness")
			conf.aggressionSharpness = std::stof(argv[++i]);

		else if (arg == "--max-time")
			conf.maxSimulationTime = std::stof(argv[++i]);

		else if (arg == "--max-finished")
			conf.maxVehiclesFinished = std::stoi(argv[++i]);

		else if (arg == "--seed")
			conf.randomSeed = std::stoi(argv[++i]);

		else if (arg == "--run")
			conf.runId = std::stoi(argv[++i]);

		else if (arg == "--experiment")
			conf.experimentName = argv[++i];
	}

	return conf;
}