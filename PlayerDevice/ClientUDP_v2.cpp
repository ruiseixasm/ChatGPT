#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"  // Replace with your server's IP address
#define SERVER_PORT 5000      // Replace with your server's port number

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Create a UDP socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up the server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    //serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_addr.s_addr = inet_pton(AF_INET, SERVER_IP, &server.sin_addr);

    // Send a message to the server
    const char* message = "Hello, server!";
    int messageLength = strlen(message);
    iResult = sendto(sock, message, messageLength, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Wait for a response from the server
    char responseBuffer[1024];
    int responseLength;
    sockaddr_in fromAddr;
    int fromAddrSize = sizeof(fromAddr);
    responseLength = recvfrom(sock, responseBuffer, sizeof(responseBuffer), 0, (sockaddr*)&fromAddr, &fromAddrSize);
    if (responseLength == SOCKET_ERROR) {
        std::cerr << "recvfrom failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Print the response from the server
    int lastCharPosition = responseLength < 1024 - 1 ? responseLength : 1024 - 1;
    responseBuffer[lastCharPosition] = '\0';
    std::cout << "Response from server: " << responseBuffer << std::endl;

    // Clean up
    closesocket(sock);
    WSACleanup();

    return 0;
}
