#pragma once

struct GLFWwindow;
struct GLFWmonitor;

namespace Caramel
{
	struct WindowData
	{
		std::string title;
		int width, height;
		bool fullscreen, vSync;
		int xPosition, yPosition;
		int refreshRate;
		bool isFocused;
	};

	class AppWindow
	{
	public:
		static GLFWwindow* GetNative();
		static GLFWmonitor* GetMonitor();
		static std::string GetTitle();
		static void SetTitle(const char* a_newName);
		static void SetFullscreen(bool a_isFull);
		static bool IsFullscreen();
		static bool ShouldClose();
		static int GetWidth() { return s_Instance->m_data.width; }
		static int GetHeight() { return s_Instance->m_data.height; }
		static bool IsFocused() { return s_Instance->m_data.isFocused; }

	private:
		AppWindow(const char* a_name, int a_width, int a_height, bool a_bFullscreen);

	private:
		GLFWwindow* m_nativeWindow;
		GLFWmonitor* m_monitor;
		WindowData m_data;


	public:
		AppWindow(AppWindow const&) = delete;
		void operator=(AppWindow const&) = delete;

		static AppWindow* CreateGLWindow(const char* a_name, int a_width, int a_height, bool a_bFullscreen);
		static void DestroyWindow();
	private:
		static AppWindow* s_Instance;
	};

	void resizeWindowCallback(GLFWwindow* a_window, int a_width, int a_height);
	void window_focus_callback(GLFWwindow* window, int focused);
}