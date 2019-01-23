#include "SandboxApp.h"

// Has to be defined in application
Caramel::Application* Caramel::CreateApplication()
{
	return new Sandbox();
}

Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::Run()
{
	CL_CORE_INFO("Application Started!");
	Application::Run();
}
