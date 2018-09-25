#include "Client.h"
#include <string>
#include <iostream>

Client::Client(u_short _port, ConnectionType type)
{
	port = _port;
	c_type = type;
}

bool Client::Start()
{
	bool ret = true;

	// Winsock init
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult == INVALID_SOCKET)
	{
		ret = false;
	}
	else
	{
		std::cout << "Client start" << std::endl;;
	}

	if (ret)
	{
		switch (c_type)
		{
			case ConnectionType::UDP:
			{
				s = socket(AF_INET, SOCK_DGRAM, 0);

				if (s == INVALID_SOCKET)
					ret = false;

				break;
			}
			case ConnectionType::TCP:
			{
				// Create socket (IPv4, stream, TCP)
				s = socket(AF_INET, SOCK_STREAM, 0);

				if (s == INVALID_SOCKET)
					ret = false;

				break;
			}
		}

		if (ret)
		{
			// Server Address
			address.sin_family = AF_INET;
			address.sin_port = htons(port);
			const char* remoteAddrStr = "127.0.0.1";

			inet_pton(AF_INET, remoteAddrStr, &address.sin_addr);

			std::cout << "Server set adress" << std::endl;;
		}

		if (ret && c_type == ConnectionType::TCP)
		{
			// Connect to server
			const int serverAddrLen = sizeof(address);

			int connectRes = connect(s, (const sockaddr*)&address, serverAddrLen);

			if (connectRes == SOCKET_ERROR)
				ret = false;
			else
				std::cout << "Connected to server" << std::endl;;
		}

		if (ret)
		{
			const int inBufferLen = 1300;
			char inBuffer[inBufferLen];

			std::string msg = "ping";

			switch (c_type)
			{
			case ConnectionType::UDP:
			{
				const int serverAddrLen = sizeof(address);
				int bytes = sendto(s, msg.c_str(), msg.size() + 1, 0, (const sockaddr*)&address, serverAddrLen);
				if (bytes >= 0)
				{
					std::cout << "Data sent" << std::endl;;
				}

				break;
			}
			case ConnectionType::TCP:
			{
				int bytes = send(s, msg.c_str(), msg.size() + 1, 0);

				if (bytes > 0)
				{
					std::cout << "Data sent" << std::endl;;
				}
				break;
			}
			}
		}

		return ret;
	}
}

bool Client::Update()
{
	bool ret = true;

	std::cout << "Client finished, press to close" << std::endl;;

	system("pause");

	ret = false;

	return ret;
}

bool Client::CleanUp()
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
