#include "Server.h"
#include <string>
#include <iostream>

Server::Server(u_short _port, ConnectionType type)
{
	port = _port;
	c_type = type;
}

bool Server::Start()
{
	bool ret = true;

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != NO_ERROR)
	{
		ret = false;
	}
	else
	{
		std::cout << "Server start" << std::endl;
	}

	if (ret)
	{
		switch (c_type)
		{
		case ConnectionType::UDP:
			s = socket(AF_INET, SOCK_DGRAM, 0);

			if (s == INVALID_SOCKET)
				ret = false;

			break;
		case ConnectionType::TCP:
			s = socket(AF_INET, SOCK_STREAM, 0);

			if (s == INVALID_SOCKET)
				ret = false;

			break;
		}
	}

	if (ret)
	{
		// Address (server)
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		address.sin_addr.S_un.S_addr = INADDR_ANY;

		const char* remoteAddrStr = "127.0.0.1";
	}

	if (ret)
	{
		// Reuse address
		int enable = 1;
		iResult = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));

		if (iResult == SOCKET_ERROR)
			ret = false;
		
	}

	if (ret)
	{
		// Bind
		iResult = bind(s, (const struct sockaddr*)&address, sizeof(address));

		if (iResult == SOCKET_ERROR)
			ret = false;
	}

	if (ret && c_type == ConnectionType::TCP)
	{
		int listenRes = listen(s, 1);

		if (listenRes == SOCKET_ERROR)
			ret = false;
	}

	if (ret && c_type == ConnectionType::TCP)
	{
		std::cout << "Waiting accept" << std::endl;

		struct sockaddr clientAddr;
		int clientAddrLen = sizeof(clientAddr);
		clientSocket = accept(s, (struct sockaddr*)&clientAddr, &clientAddrLen);

		if (clientSocket == INVALID_SOCKET)
			ret = false;
		else
			std::cout << "Accept done" << std::endl;
	}

	if (ret)
	{
		std::cout << "Waiting for client data... " << std::endl;;
	}

	return ret;
}

bool Server::Update()
{
	bool ret = true;

	// Input buffer
	const int inBufferLen = 1300;
	char inBuffer[inBufferLen];

	int fromlen = sizeof(sockaddr);

	switch (c_type)
	{
	case ConnectionType::UDP:
	{
		int fromAddrLen = sizeof(fromAddr);

		int bytes = recvfrom(s, inBuffer, inBufferLen, 0, &fromAddr, &fromAddrLen);
		if (bytes >= 0)
		{
			std::string val = inBuffer;

			if (val == "ping")
			{
				std::cout << "Data recieved " << std::endl;;

				std::cout << "Server finished, press to close" << std::endl;;

				system("pause");

				ret = false;
			}
		}
		break;
	}
	case ConnectionType::TCP:
	{
		int bytes = recv(clientSocket, inBuffer, inBufferLen, 0);

		if (bytes > 0)
		{
			std::string val = inBuffer;

			if (val == "ping")
			{
				std::cout << "Data recieved " << std::endl;;

				std::cout << "Server finished, press to close" << std::endl;;

				system("pause");

				ret = false;
			}
		}
		break;
	}
	default:
		break;
	}

	return ret;
}

bool Server::CleanUp()
{
	bool ret = true;

	int iResult = closesocket(s);

	if (iResult != NO_ERROR)
	{
		ret = false;
	}

	iResult = WSACleanup();

	if (iResult != NO_ERROR)
	{
		ret = false;
	}

	return ret;
}
