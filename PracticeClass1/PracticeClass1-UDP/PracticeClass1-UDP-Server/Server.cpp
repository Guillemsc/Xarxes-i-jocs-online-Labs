#include "Server.h"

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

	if (ret)
	{
		switch (c_type)
		{
		case ConnectionType::UDP:
			s = socket(AF_INET, SOCK_DGRAM, 0);
			break;
		case ConnectionType::TCP:
			s = socket(AF_INET, SOCK_STREAM, 0);
			break;
		}
	}

	if (ret)
	{
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		const char* remoteAddrStr = "127.0.0.1";

		address.sin_addr.S_un.S_addr = INADDR_ANY;
	}

	if (ret)
	{
		int enable = 1;
		iResult = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));

		if (iResult != SOCKET_ERROR)
		{
			ret = false;
		}
	}

	if (ret)
	{
		iResult = bind(s, (const struct sockaddr*)&address, sizeof(address));

		if (iResult != SOCKET_ERROR)
		{
			ret = false;
		}
	}

	return ret;
}

bool Server::Update()
{
	bool ret = true;

	char* buf = new char[10];

	int fromlen = sizeof(sockaddr);

	recvfrom(s, buf, 10, 0, (struct sockaddr*)&address, &fromlen);

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
