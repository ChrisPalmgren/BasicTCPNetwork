#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "WS2_32.lib")

void main()
{
	// Init WinSock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		std::cerr << "Can't init winsock. Exiting." << std::endl;
		return;
	}

	// Create Socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket. Exiting." << std::endl;
		return;
	}
	else
	{
		std::cout << "Listening for request..." << std::endl;
	}

	// Bind socket on an ip adress and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell WinSock tje socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " is connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " is connected on port " << ntohs(client.sin_port) << std::endl;
	}

	// Close listening socket
	closesocket(listening);

	// While loop: Accept and echo message back to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);
		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error in recv(). Exiting." << std::endl;
			break;
		}
		if (bytesReceived == 0)
		{
			std::cout << "Client Disconnected" << std::endl;
			break;
		}

		std::cout << host << ": " << std::string(buf, 0, bytesReceived) << std::endl;

		// Echo massage back to client
		send(clientSocket, buf, bytesReceived + 1, 0);

		
	}

	// Close Socket
	closesocket(clientSocket);

	// Clean WinSock
	WSACleanup();
}