#include "MyApplication.h"
#include "Defines.h"
#include <iostream>

// Application globals
int* g_ScreenWidth;
int* g_ScreenHeight;

// main that controls the creation/destruction of an application
int main(int argc, char* argv[])
{
#pragma region Program start prompt

	FILE* src = nullptr;
	FILE* dst = fopen("imgui.ini", "w");

	int w, h;
	bool isFullScreen;
	std::cout << "Select Resolution:\n1. 2560x1440\n2. 1920x1080\n3. 1280x720\n";

	int input = -1;

	bool validRes = false;
	while (!validRes)
	{
		std::cin >> input;
		switch (input)
		{
		case 1:
		{
			w = 2560;
			h = 1440;
			validRes = true;
			src = fopen("config/1440/imgui.ini", "r");
			break;
		}

		case 2:
		{
			w = 1920;
			h = 1080;
			validRes = true;
			src = fopen("config/1080/imgui.ini", "r");
			break;
		}

		case 3:
		{
			w = 1280;
			h = 720;
			validRes = true;
			src = fopen("config/720/imgui.ini", "r");
			break;
		}
		}
	}

	std::cout << "Fullscreen?\n1. Yes\n2. No\n";

	bool validScreen = false;
	while (!validScreen)
	{
		std::cin >> input;
		switch (input)
		{
		case 1:
		{
			isFullScreen = true;
			validScreen = true;
			break;
		}
		case 2:
		{
			isFullScreen = false;
			validScreen = true;
			break;
		}
		default:
			break;
		}
	}

	while (!feof(src))
	{
		fputc(fgetc(src), dst);
	}

	fclose(src);
	fclose(dst);

	g_ScreenWidth = &w;
	g_ScreenHeight = &h;

	system("cls");

#pragma endregion

	// explicitly control the creation of our application
	MyApplication* app = new MyApplication();
	app->run("S1608758: Callum_Haines", *g_ScreenWidth, *g_ScreenHeight, isFullScreen);

	// explicitly control the destruction of our application
	delete app;

	return 0;
}