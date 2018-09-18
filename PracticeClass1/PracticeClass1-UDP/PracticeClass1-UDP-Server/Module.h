#ifndef _MODULE_
#define _MODULE_

class Module
{
public:
	Module() {}

	virtual bool Start() { return true; }
	virtual bool Update() { return true; }
	virtual bool CleanUp() { return true; }
};

#endif // !_MODULE_
