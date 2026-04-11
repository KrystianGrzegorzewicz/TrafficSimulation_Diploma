#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "core/Simulation.h"

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5555);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 1);

    std::cout << "Waiting for Python client...\n";
    SOCKET client_socket = accept(server_fd, NULL, NULL);
    std::cout << "Client connected!\n";

    Simulation sim;

    auto last = std::chrono::high_resolution_clock::now();

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(now - last).count();
        last = now;

        sim.step(dt);

        std::string msg = sim.getWorldJson();
        send(client_socket, msg.c_str(), msg.size(), 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}