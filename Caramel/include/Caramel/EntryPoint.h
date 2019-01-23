#pragma once


#ifdef CL_PLATFORM_WINDOWS

extern Caramel::Application* Caramel::CreateApplication();

int main(int argc, char** argv)
{
	Caramel::Log::Init();
	CL_CORE_INFO("Initialised Log!");

	auto app = Caramel::CreateApplication();
	CL_CORE_WARN("Starting Application");
	app->Run();
	delete app;
}

#endif