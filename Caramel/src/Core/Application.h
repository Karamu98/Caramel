#pragma once

// BUG: This has to be included to stop errors when building Game
#include "clpch.h"

#include "Render/Window.h"

namespace Caramel
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		bool CreateApp(const char* a_name, int a_width, int a_height, bool a_bFullscreen);
		void Run(const char* a_name, int a_width, int a_height, bool a_bFullscreen);
		void Quit() { m_isRunning = false; }

	protected:

		// Client implementation
		virtual bool OnCreate() = 0;
		virtual void Update(float a_deltaTime) = 0;
		virtual void ImDraw() = 0;
		virtual void Draw() = 0;
		virtual void Destroy() = 0;

		AppWindow* m_window;
		bool m_isRunning;

	private:
		void ClientDraw();
		void ClientGUI();
		void ClientDestroy();

	public:
		static void SetTimeDilation(float a_newDilation);
	private:
		static float sm_timeDilation;
	};
}