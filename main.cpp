#include <winsock2.h>
#include <ws2tcpip.h>
#include <print>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include "simulation/Simulation.h"
#include "core/Config.h"

#pragma comment(lib, "Ws2_32.lib")

// ---------------------------------------------------------------------------
// Config parsing (identical to original)
// ---------------------------------------------------------------------------

static Config parseConfig()
{
	Config conf;
	int input = 1;

	std::print("Load default config? 1 - Yes, 0 - No: ");
	std::cin >> input;

	if (input != 1)
	{
		std::print("Connect to graphic interface? 1 - Yes, 0 - No: ");
		std::cin >> input;
		if (input == 0) conf.gui = false;

		std::print("Save data to csv? 1 - Yes, 0 - No: ");
		std::cin >> input;
		if (input == 1) conf.saveCsv = true;

		std::print("Set simulation speed (e.g. 1.0 for normal): ");
		std::cin >> conf.simSpeed;

		std::print("Set autonomous vehicle rate (0.0 - 1.0): ");
		std::cin >> conf.AVRate;

		std::print("Set junction sample (1, 2, 3, 4): ");
		std::cin >> conf.junction;
	}

	return conf;
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	const Config conf = parseConfig();

	// All time logic now lives inside Simulation::step()
	Simulation sim(conf.spawnRate, conf.junction, conf.saveCsv, conf.AVRate);

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


		// ---- TCP server setup (unchanged from original) ----
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

		// ---- Render loop ----
		float accumulator = 0.f;
		float sendTimer = 0.f;
		auto  last = std::chrono::high_resolution_clock::now();

		while (true)
		{
			auto  now = std::chrono::high_resolution_clock::now();
			float realDt = std::chrono::duration<float>(now - last).count();
			last = now;

			if (realDt > maxRealDt) realDt = maxRealDt;

			// Scale by simSpeed so fast/slow-motion works
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
				send(client_socket,
					msg.c_str(),
					static_cast<int>(msg.size()),
					0);
				sendTimer = 0.f;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		closesocket(client_socket);
		closesocket(server_fd);
	}
	else
	{
		std::print("Started headless\n");
		while (true)
			sim.step(fixedDt);
	}

	WSACleanup();
	return 0;
}