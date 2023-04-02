// C++ UDP Transmitter

#include "Network.h"
#include "iostream"

#pragma once

int main()
{
    std::string IP = "127.0.0.1";
    int PORT = 8888;

    try
    {
        WSASession Session;
        UDPSocket Socket;
        std::string data = "hello world";
        char buffer[100];

        while (1)
        {
            std::cout << "Enter data to transmit : " << std::endl;
            std::getline(std::cin, data);
            Socket.SendTo(IP, PORT, data.c_str(), data.size());
        }
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what();
    }
}