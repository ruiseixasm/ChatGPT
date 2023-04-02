#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // Create a socket for sending data
    SOCKET sendingSocket;
    sendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sendingSocket == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up the destination address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(27015);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // Send some data to the server
    char sendData[] = "Hello, server!";
    iResult = sendto(sendingSocket, sendData, sizeof(sendData), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "sendto failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sendingSocket);
        WSACleanup();
        return 1;
    }

    // Clean up
    closesocket(sendingSocket);
    WSACleanup();
    return 0;
}
