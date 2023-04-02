#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // for inet_pton
#include <chrono>
#include <ratio>

#pragma comment(lib, "ws2_32.lib")

#define CLIENT "192.168.31.255" // BROADCAST
#define PORT 3108

bool OutdatedPeriod(unsigned long duration = 0) {

    static unsigned long long valid_duration = 0;
    static auto last_time = std::chrono::system_clock::now();

    if (duration > 0) {
        valid_duration = duration;
        last_time = std::chrono::system_clock::now();
    }
    else {
        auto now = std::chrono::system_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time);
        unsigned long long duration = elapsed_ms.count();

        if (duration > valid_duration) {
            return true;
        }
    }

    return false;
}

unsigned long long ArduinoMillis() {

    auto now_system = std::chrono::system_clock::now();
    auto now_absolute = std::chrono::time_point_cast<std::chrono::milliseconds>(now_system);
    auto now_relative = now_absolute.time_since_epoch();
    return now_relative.count();
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Create a UDP socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up the server address to send to
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, CLIENT, &clientAddr.sin_addr) != 1) {
        std::cerr << "inet_pton failed with error." << std::endl;
        WSACleanup();
        return 1;
    }

    while (true) {

        if (OutdatedPeriod()) {

            // Send a message to the server
            std::string message = "Hello, server!";
            if (sendto(sock, message.c_str(), (int)message.length(), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
                std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
                closesocket(sock);
                WSACleanup();
                return 1;
            }

            std::cout << "Message sent to server: " << message << std::endl;
            OutdatedPeriod(5*1000);

        }

    }

    // Clean up
    closesocket(sock);
    WSACleanup();

    return 0;
}
