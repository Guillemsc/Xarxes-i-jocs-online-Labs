#include "Client.h"

Client::Client(u_short _port, ConnectionType type)
{
	port = _port;
	c_type = type;
}

bool Client::Start()
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

		iResult = inet_pton(AF_INET, remoteAddrStr, &address.sin_addr);

		if (iResult != 1)
		{
			ret = false;
		}
	}

	if (ret)
	{
		const char* msg = "ping";

		switch (c_type)
		{
		case ConnectionType::UDP:
			sendto(s, msg, sizeof(msg), 0, (const struct sockaddr*)&address, sizeof(address));
			break;
		case ConnectionType::TCP:
			s = socket(AF_INET, SOCK_STREAM, 0);
			break;
		}
	}

	return ret;
}

bool Client::Update()
{
	bool ret = true;

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
