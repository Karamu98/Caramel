#include <Caramel.h>

class Sandbox : public Caramel::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Caramel::Application* Caramel::CreateApplication()
{
	return new Sandbox();
}