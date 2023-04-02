#include <Windows.h>
#include <winsock2.h> // gives lots of erros if not implemented
#include <iostream>

int main() {
	std::cout << "\t\t--------- UDP Client ----------" << std::endl;
	std::cout << std::endl;

	// Local Variables
	WSADATA		WinSockData;
	int			iWsaStartup;
	int			iWsaCleanup;

	SOCKET		UDPSocketClient;
	struct		sockaddr_in			UDPServer;

	char		Buffer[512] = "Hello from Client!";
	int			iSendTo;

	int			iBufferLen = strlen(Buffer) + 1;
	int			iUDDPServerLen = sizeof(UDPServer);
	int			iCloseSocket;


}