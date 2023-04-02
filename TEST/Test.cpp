#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <iostream>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define CLIENT "127.0.0.1" // 2130706433
#define PORT 3108
#define BUFFER_SIZE 1024

int main()
{

    int iResult = 0;

    WSADATA wsaData;

    SOCKET RecvSocket;
    struct sockaddr_in RecvAddr;

    char RecvBuf[BUFFER_SIZE];

    struct sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof(SenderAddr);

    //-----------------------------------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error %d\n", iResult);
        return 1;
    }
    //-----------------------------------------------
    // Create a receiver socket to receive datagrams
    RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (RecvSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error %d\n", WSAGetLastError());
        return 1;
    }
    //-----------------------------------------------
    // Bind the socket to any address and the specified port.
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port = htons(PORT);
    RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    iResult = bind(RecvSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
    if (iResult != 0) {
        wprintf(L"bind failed with error %d\n", WSAGetLastError());
        return 1;
    }

    while (true)
    {

        //-----------------------------------------------
        // Call the recvfrom function to receive datagrams | AWAITS FOR THE MESSAGE
        // on the bound socket.
        wprintf(L"Receiving datagrams...\n");
        iResult = recvfrom(RecvSocket, RecvBuf, BUFFER_SIZE, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
        //iResult = recvfrom(RecvSocket, RecvBuf, BufLen, 0, reinterpret_cast<SOCKADDR*>(&SenderAddr), &SenderAddrSize);
        if (iResult == SOCKET_ERROR) {
            wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
        }

        static char RecvAddress[16];
        static int RecvAddressLen = 16;
        PCSTR address_string = inet_ntop(AF_INET, &SenderAddr.sin_addr, RecvAddress, RecvAddressLen);

        static int lastChar = iResult < BUFFER_SIZE ? iResult : BUFFER_SIZE - 1;
        RecvBuf[lastChar] = '\0';
        std::string input(RecvBuf); // Creates a string from an array of chars! | NOT NECESSARY
        std::cout << "Received " << iResult << " bytes from " << address_string << ": " << input << std::endl;
        //wprintf(L"Received message!\n");

    }

    //-----------------------------------------------
    // Close the socket when finished receiving datagrams | WHERE IT RECEIVES THE DATA!
    wprintf(L"Finished receiving. Closing socket.\n");
    iResult = closesocket(RecvSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    //-----------------------------------------------
    // Clean up and exit.
    wprintf(L"Exiting.\n");
    WSACleanup();
    return 0;
}