#pragma once

// This is the implementation for windows entry, this style allows for other platforms
#ifdef CL_PLATFORM_WINDOWS

extern Caramel::Application* Caramel::CreateApplication();

// This is the entry point for the engine.
// This is defined here to keep application control on the engine side
int main(int argc, char** argv)
{
	Caramel::InitializeCore();
	Caramel::Application* app = Caramel::CreateApplication();
	CL_CORE_ASSERT(app, "Client Application is null!");
	app->Run();
	delete app;
	Caramel::ShutdownCore();
}

#endif
