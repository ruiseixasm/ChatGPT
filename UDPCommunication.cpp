#include "UDPCommunication.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

UDPCommunication::UDPCommunication(int port) {
    // Create a UDP socket
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }

    // Bind the socket to a port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket to port " << port << std::endl;
        exit(1);
    }
}

UDPCommunication::~UDPCommunication() {
    close(socket_fd);
}

void UDPCommunication::send(const std::string& message, const std::string& ip_address, int port) {
    // Set up the server address to send the message to
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    if (inet_aton(ip_address.c_str(), &client_addr.sin_addr) == 0) {
        std::cerr << "Error converting IP address " << ip_address << std::endl;
        return;
    }

    // Send the message
    if (sendto(socket_fd, message.c_str(), message.length(), 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        std::cerr << "Error sending message" << std::endl;
    }
}

std::string UDPCommunication::receive(int buffer_size) {
    // Receive a message
    char buffer[buffer_size];
    memset(buffer, 0, buffer_size);
    socklen_t client_len = sizeof(client_addr);
    int n = recvfrom(socket_fd, buffer, buffer_size - 1, 0, (struct sockaddr*)&client_addr, &client_len);
    if (n < 0) {
        std::cerr << "Error receiving message" << std::endl;
        return "";
    }

    // Return the message as a string
    return std::string(buffer);
}
