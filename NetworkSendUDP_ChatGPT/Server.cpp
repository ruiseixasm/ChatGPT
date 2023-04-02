#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // for inet_pton
#include <chrono>
#include <ratio>

#pragma comment(lib, "ws2_32.lib")

#define CLIENT "192.168.31.255" // BROADCAST
#define PORT 3108

unsigned long millis() {
    auto current_time = std::chrono::steady_clock::now();

    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time);

    //return std::chrono::duration_cast<std::chrono::milliseconds>(current_time).count();
    //return std::chrono::duration_cast<std::chrono::milliseconds>(current_time).count();

    return int_ms.count();
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

    while(true) {
        static auto last_time = std::chrono::steady_clock::now();

        // Send a message to the server
        std::string message = "Hello, server!";
        if (sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
            std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        std::cout << "Message sent to server: " << message << std::endl;

    }

    // Clean up
    closesocket(sock);
    WSACleanup();

    return 0;
}
