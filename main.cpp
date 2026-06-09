#include "simulation/Simulation.h"
#include "vehicles/DriverPersonalityGenerator.h"
#include "core/Config.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <print>

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	const Config conf = parseConfig(argc, argv);

	if (conf.randomSeed != 0)
		srand(conf.randomSeed);
	else
		srand((unsigned)time(nullptr));

	DriverPersonalityGenerator::configure(
		conf.aggressionBias,
		conf.aggressionSharpness
	);

	Simulation sim(
		conf.spawnRate,
		conf.junction,
		conf.saveCsv,
		conf.AVRate,
		conf
	);

	const float fixedDt = 0.01f;
	const float maxRealDt = 0.05f;
	const float sendInterval = 1.f / 60.f;

	if (conf.gui)
	{
		std::print("Started gui\n");

		int result = system("start python visualizer.py");

		if (result != 0) {
			// handle error
		}

		SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

		sockaddr_in address{};
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(5555);

		bind(server_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
		listen(server_fd, 1);

		std::println("Waiting for Python client...");
		SOCKET client_socket = accept(server_fd, nullptr, nullptr);
		std::println("Client connected!");

		float accumulator = 0.f;
		float sendTimer = 0.f;
		auto  last = std::chrono::high_resolution_clock::now();

		while (true)
		{
			auto  now = std::chrono::high_resolution_clock::now();
			float realDt = std::chrono::duration<float>(now - last).count();
			last = now;

			if (realDt > maxRealDt) realDt = maxRealDt;

			accumulator += realDt * conf.simSpeed;

			while (accumulator >= fixedDt)
			{
				sim.step(fixedDt);
				accumulator -= fixedDt;
			}

			sendTimer += realDt;
			if (sendTimer >= sendInterval)
			{
				std::string msg = sim.getWorldJson();
				send(client_socket, msg.c_str(), static_cast<int>(msg.size()), 0);
				sendTimer = 0.f;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (conf.maxSimulationTime > 0)
			{
				if (sim.getCurrentTime() >= conf.maxSimulationTime)
					break;
			}

			if (conf.maxVehiclesFinished > 0)
			{
				if (sim.getFinishedVehicles() >= conf.maxVehiclesFinished)
					break;
			}
		}

		closesocket(client_socket);
		closesocket(server_fd);
	}
	else
	{
		std::print("Started headless\n");
		while (true)
		{
			sim.step(fixedDt);

			if (conf.maxSimulationTime > 0)
			{
				if (sim.getCurrentTime() >= conf.maxSimulationTime)
					break;
			}

			if (conf.maxVehiclesFinished > 0)
			{
				if (sim.getFinishedVehicles() >= conf.maxVehiclesFinished)
					break;
			}
		}
	}
	WSACleanup();
	return 0;
}