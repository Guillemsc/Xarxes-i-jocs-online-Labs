#include <iostream>
#include <vector>

#include "Module.h"

#include "Server.h"

int main(int argc, char **argv)
{
	int ret = 0;

	std::vector<Module*> modules;

	Server* client = new Server(8000, ConnectionType::UDP);

	modules.push_back(client);

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		ret = (*it)->Start();
	}

	while (ret)
	{
		for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		{
			ret = (*it)->Update();
		}
	}

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}