#ifndef UDP_COMMUNICATION_H
#define UDP_COMMUNICATION_H

#include <string>

class UDPCommunication {
private:
    int socket_fd;
    struct sockaddr_in server_addr, client_addr;

public:
    UDPCommunication(int port);
    virtual ~UDPCommunication();

    void send(const std::string& message, const std::string& ip_address, int port);
    std::string receive(int buffer_size = 1024);
};

#endif /* UDP_COMMUNICATION_H */
