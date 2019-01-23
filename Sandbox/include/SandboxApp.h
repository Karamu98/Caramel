#ifndef __SANDBOX_H_
#define __SANDBOX_H_

#include <Caramel.h>

class Sandbox : public Caramel::Application
{
public:
	Sandbox();
	~Sandbox();

	void Run(); // Overrided to Supply debug "TEMP"

};

#endif