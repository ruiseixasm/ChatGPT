#include "udp_client.h"

UDPClient::UDPClient() {
    // Initialize Winsock
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        std::cerr << "WSAStartup failed with error: " << res << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

bool UDPClient::initialize(std::string ip_address, int port) {
    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &(server_addr.sin_addr));

    // Connect to server
    int res = connect(sock, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if (res == SOCKET_ERROR) {
        std::cerr << "connection failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }

    return true;
}

bool UDPClient::send(std::string message) {
    // Send message to server
    int res = send(sock, message.c_str(), message.length(), 0);
    if (res == SOCKET_ERROR) {
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }

    return true;
}

void UDPClient::close() {
    // Close socket and cleanup
    closesocket(sock);
    WSACleanup();
}
