#include <winsock2.h>
#include <ws2tcpip.h>
#include <print>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "core/Simulation.h"
#include "core/Config.h"

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    Config conf;
	int input;
    std::print("Load default config? 1 - Yes, 0 - No: ");
	std::cin >> input;
    if (input != 1)
    {
        std::print("Connect to graphic interface? 1 - Yes, 0 - No: ");
        std::cin >> input;
        if (input == 0)
			conf.gui = false;
        std::print("Save data to csv? 1 - Yes, 0 - No: ");
        std::cin >> input;
        if (input == 1)
            conf.saveCsv = true;
        std::print("Set simulation speed (e.g., 1.0 for normal speed): ");
        std::cin >> conf.simSpeed;
        std::print("Set simulation junction sample (1, 2): ");
        std::cin >> conf.junction;
    }

    const float fixedDt = 0.01f;
    Simulation sim = Simulation(conf.spawnRate, conf.junction, conf.saveCsv);

    if (conf.gui)
    {
        std::print("Started gui\n");
        SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(5555);

        bind(server_fd, (sockaddr*)&address, sizeof(address));
        listen(server_fd, 1);

        std::println("Waiting for Python client...\n");
        SOCKET client_socket = accept(server_fd, NULL, NULL);
        std::println("Client connected!\n");

        auto last = std::chrono::high_resolution_clock::now();
        float accumulator = 0.0f;
        const float maxDt = 0.05f;
        const float sendInterval = 1.0f / 60.0f;

        float sendTimer = 0.0f;

        while (true) {
            auto now = std::chrono::high_resolution_clock::now();
            float realDt = std::chrono::duration<float>(now - last).count();
            last = now;

            if (realDt > maxDt)
                realDt = maxDt;
            float simDt = realDt * conf.simSpeed;

            accumulator += simDt;
            while (accumulator >= fixedDt) {
                sim.step(fixedDt);
                accumulator -= fixedDt;
            }
            sendTimer += realDt;

            if (sendTimer >= sendInterval) {
                std::string msg = sim.getWorldJson();
                send(client_socket, msg.c_str(), msg.size(), 0);
                sendTimer = 0.0f;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    } else {
        std::print("Started no gui\n");
        while (true) {
            sim.step(fixedDt);
        }
    }
}