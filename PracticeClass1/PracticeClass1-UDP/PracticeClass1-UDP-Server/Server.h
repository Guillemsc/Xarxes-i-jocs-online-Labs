#ifndef _SERVER_
#define _SERVER_

#include "Module.h"

// Windows sockets dependences
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
// ---------------------------

enum ConnectionType
{
	UDP,
	TCP,
};

class Server : public Module
{
	struct sockaddr_in
	{
		short sin_family;
		unsigned short sin_port;
		struct in_addr sin_addr;
		char sin_zero[8];
	};

public:
	Server(u_short port, ConnectionType type);

	bool Start();
	bool Update();
	bool CleanUp();

private:
	u_short port;

	SOCKET s;
	sockaddr_in address;

	ConnectionType c_type;

	SOCKET clientSocket;

	struct sockaddr fromAddr;
};

#endif //_CLIENT_