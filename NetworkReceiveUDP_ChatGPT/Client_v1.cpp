#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // For inet_pton

#pragma comment(lib, "Ws2_32.lib")

#define CLIENT "127.0.0.1" // 2130706433
#define PORT 3108
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    SOCKET recvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (recvSocket == INVALID_SOCKET)
    {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in recvAddr = { 0 };
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(PORT);
    int recv_address = inet_pton(AF_INET, CLIENT, &recvAddr.sin_addr); // IMPORTAT FUNCTION HERE

    if (bind(recvSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
    {
        std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(recvSocket);
        WSACleanup();
        return 1;
    }

    char buffer[BUFFER_SIZE];
    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);
    int recvLen;

    while (true)
    {
        recvLen = recvfrom(recvSocket, buffer, BUFFER_SIZE, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);
        if (recvLen == SOCKET_ERROR)
        {
            std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
            closesocket(recvSocket);
            WSACleanup();
            return 1;
        }

        PCSTR address_string = inet_ntop(AF_INET, &senderAddr.sin_addr, buffer, sizeof buffer);
        int lastCharPosition = recvLen < BUFFER_SIZE ? recvLen : BUFFER_SIZE - 1;
        //buffer[lastCharPosition] = '\0';
        //std::cout << "Received " << recvLen << " bytes from " << inet_ntoa(senderAddr.sin_addr) << ": " << buffer << std::endl;
        std::cout << "Received " << recvLen << " bytes from " << address_string << ": " << buffer << std::endl;
    }

    closesocket(recvSocket);
    WSACleanup();
    return 0;
}
