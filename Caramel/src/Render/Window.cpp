#include "clpch.h"
#include "Window.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Core/Log.h"

namespace Caramel
{
	AppWindow* AppWindow::s_Instance = nullptr;

	AppWindow::AppWindow(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
	{
		m_data.fullscreen = a_bFullscreen;
		m_data.width = a_width, m_data.height = a_height;
		m_data.title = a_name;
		m_data.vSync = true;

		// Initialise glfw
		if (glfwInit() != GL_TRUE)
		{
			CL_CORE_INFO("Unable to initialize GLFW");
		}

		// Create the window with glfw
		m_nativeWindow = glfwCreateWindow(a_width, a_height, a_name, (a_bFullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
		if (m_nativeWindow == nullptr)
		{
			CL_CORE_INFO("Unable to create a GLFW Window");
			glfwTerminate();
		}

		// Set it as active
		glfwMakeContextCurrent(m_nativeWindow);

		// Load glad
		if (!gladLoadGL()) {
			glfwDestroyWindow(m_nativeWindow);
			glfwTerminate();
		}

		m_monitor = glfwGetPrimaryMonitor();
		m_data.isFocused = true;

		// Print GL version to console
		CL_CORE_TRACE("\n=====\nOpenGl:\nVendor: {0}\nRenderer: {1}\nVersion: {2}\n=====\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));

		// Give OpenGL our user data
		glfwSetWindowUserPointer(m_nativeWindow, &m_data);

		// Subscribe to opengl callbacks
		glfwSetFramebufferSizeCallback(m_nativeWindow, resizeWindowCallback);
		glfwSetWindowFocusCallback(m_nativeWindow, window_focus_callback);
	}

	GLFWwindow* AppWindow::GetNative()
	{
		return s_Instance->m_nativeWindow;
	}

	GLFWmonitor* AppWindow::GetMonitor()
	{
		return s_Instance->m_monitor;
	}

	std::string AppWindow::GetTitle()
	{
		return s_Instance->m_data.title;
	}

	void AppWindow::SetTitle(const char* a_newName)
	{
		s_Instance->m_data.title = a_newName;
		glfwSetWindowTitle(s_Instance->m_nativeWindow, a_newName);
	}

	void AppWindow::SetFullscreen(bool a_isFull)
	{
		if (a_isFull == IsFullscreen())
		{
			return;
		}

		static int width, height, x, y;
		if (a_isFull)
		{
			// Backup window position and window size
			glfwGetWindowPos(s_Instance->m_nativeWindow, &x, &y);
			glfwGetWindowSize(s_Instance->m_nativeWindow, &width, &height);

			// Get current resolution of monitor
			auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			s_Instance->m_data.refreshRate = mode->refreshRate;

			// Go full screen
			glfwSetWindowMonitor(s_Instance->m_nativeWindow, s_Instance->m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			glfwSetWindowMonitor(s_Instance->m_nativeWindow, nullptr,
				x, y,
				width, height,
				s_Instance->m_data.refreshRate);
		}
	}


	bool AppWindow::IsFullscreen()
	{
		s_Instance->m_data.fullscreen = glfwGetWindowMonitor(s_Instance->m_nativeWindow) != nullptr;
		return s_Instance->m_data.fullscreen;
	}

	bool AppWindow::ShouldClose()
	{
		return glfwWindowShouldClose(s_Instance->m_nativeWindow);
	}

	AppWindow* AppWindow::CreateGLWindow(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new AppWindow(a_name, a_width, a_height, a_bFullscreen);
		}
		else
		{
			CL_CORE_ERROR("Window already exists.");
		}
		return s_Instance;
	}

	void AppWindow::DestroyWindow()
	{
		if (s_Instance != nullptr)
		{
			glfwDestroyWindow(s_Instance->GetNative());
			delete s_Instance;
		}
	}

	void resizeWindowCallback(GLFWwindow* a_window, int a_width, int a_height)
	{
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(a_window);
		data->width = a_width, data->height = a_height;
	}

	void window_focus_callback(GLFWwindow* window, int focused)
	{
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (focused)
		{
			// The window gained input focus
			data->isFocused = true;
		}
		else
		{
			// The window lost input focus
			data->isFocused = false;
		}
	}
}