#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

void main()
{
	std::string IP_Adress = "127.0.0.1";
	int port = 54000;

	// Init WinSock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);

	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start winsock. ERROR " << wsResult << std::endl;
		return;
	}
	// Create socket
	SOCKET Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket. ERROR " << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	char answer;
	std::string dummy;
	bool pass = false;

	// Fill in hint struct
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, IP_Adress.c_str(), &hint.sin_addr);
	while (pass == false)
	{	
		// Connect to server
		std::cout << "Request connection from server (Y / N): ";
		std::cin >> answer;
		std::getline(std::cin, dummy);
		int conResult;
		switch (answer)
		{
		case 'y': case 'Y':
			conResult = connect(Sock, (sockaddr*)&hint, sizeof(hint));
			if (conResult == SOCKET_ERROR)
			{
				std::cerr << "Can't connect to server. ERROR " << WSAGetLastError() << std::endl;
				std::cin.get();
				closesocket(Sock);
				WSACleanup();
				return;
			}
			else
			{
				std::cout << "Server has accepted your request." << std::endl;
				std::cin.get();
				system("cls");
				pass = true;
			} break;
		case 'n': case 'N':
			return;
		default:
			system("cls");
		}
	}
	
	
	// Do-while loop to send and recieve data
	char buf[4096];
	std::string userInput;

	do
	{
		// Prompt user for some text
		std::cout << "> ";
		std::getline(std::cin, userInput);

		if (userInput.size() > 0)
		{
			// Send text
			int sendResult = send(Sock, userInput.c_str(), userInput.size() + 1, 0);
		//	if (sendResult != SOCKET_ERROR)
		//	{
		//		// Wait for response
		//		ZeroMemory(buf, 4096);
		//		int bytesReceived = recv(Sock, buf, 4096, 0);
		//		if (bytesReceived > 0)
		//		{
		//			// Echo response to console
		//			std::cout << "SERVER > " << std::string(buf, 0, bytesReceived) << std::endl;
		//		}
		//	}
		}

	} while (userInput.size() > 0);
	// Close down all
	closesocket(Sock);
	WSACleanup();
}