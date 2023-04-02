#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

class UDPClient {
public:
    UDPClient();
    bool initialize(std::string ip_address, int port);
    bool send(std::string message);
    void close();

private:
    SOCKET sock;
    SOCKADDR_IN server_addr;
};

#endif
