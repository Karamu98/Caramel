#pragma once


#ifdef CL_PLATFORM_WINDOWS

extern Caramel::Application* Caramel::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Caramel::CreateApplication();
	app->Run();
	delete app;
}

#endif