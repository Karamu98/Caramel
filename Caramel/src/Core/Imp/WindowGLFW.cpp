#include "clpch.h"
#include "WindowGLFW.h"
//#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "Core/Log.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"

namespace Caramel::Implementation
{
	namespace
	{
		GLFWmonitor* GetWindowMonitor(GLFWwindow* window)
		{
			int monitorCount;
			int winPosX, winPosY, winWidth, winHeight;
			int monitorPosX, monitorPosY, monitorWidth, monitorHeight;
			int overlap, bestoverlap;
			GLFWmonitor* bestmonitor;
			GLFWmonitor** monitors;
			const GLFWvidmode* mode;

			bestoverlap = 0;
			bestmonitor = NULL;

			glfwGetWindowPos(window, &winPosX, &winPosY);
			glfwGetWindowSize(window, &winWidth, &winHeight);
			monitors = glfwGetMonitors(&monitorCount);

			for (int i = 0; i < monitorCount; i++)
			{
				mode = glfwGetVideoMode(monitors[i]);
				glfwGetMonitorPos(monitors[i], &monitorPosX, &monitorPosY);
				monitorWidth = mode->width;
				monitorHeight = mode->height;

				overlap =
					std::max(0, std::min(winPosX + winWidth, monitorPosX + monitorWidth) - std::max(winPosX, monitorPosX)) *
					std::max(0, std::min(winPosY + winHeight, monitorPosY + monitorHeight) - std::max(winPosY, monitorPosY));

				if (bestoverlap < overlap)
				{
					bestoverlap = overlap;
					bestmonitor = monitors[i];
				}
			}

			return bestmonitor;
		}

#define GETDATA (Caramel::Implementation::WindowGLFWData*)glfwGetWindowUserPointer(window)

		void OnWindowResize(GLFWwindow* window, int width, int height)
		{
			auto windowData = GETDATA;

			windowData->OldWinWidth = windowData->Width;
			windowData->OldWinHeight = windowData->Height;
			windowData->Width = width;
			windowData->Height = height;
			windowData->Fullscreen = glfwGetWindowMonitor(window) != nullptr;

			WindowResizeEvent event((unsigned int)width, (unsigned int)height, windowData->Fullscreen);
			windowData->Width = width;
			windowData->Height = height;
			windowData->EventCallback(event);
		}

		void OnCloseWindow(GLFWwindow* window)
		{
			auto windowData = GETDATA;

			windowData->ShouldClose = true;

			WindowCloseEvent event;
			windowData->EventCallback(event);
		}

		void OnWindowMove(GLFWwindow* window, int windowX, int windowY)
		{
			auto windowData = GETDATA;

			windowData->OldWinPosX = windowData->WindowXPos;
			windowData->OldWinPosY = windowData->WindowYPos;
			windowData->WindowXPos = windowX;
			windowData->WindowYPos = windowY;

			WindowMoveEvent event(windowX, windowY);
			windowData->EventCallback(event);
		}

		void OnWindowFocus(GLFWwindow* window, int focused)
		{
			auto windowData = GETDATA;

			windowData->Focused = focused;

			WindowFocusEvent event(focused);
			windowData->EventCallback(event);
		}

		void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto data = GETDATA;

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data->EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data->EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data->EventCallback(event);
				break;
			}
			}
		}

		void OnChar(GLFWwindow* window, unsigned int codepoint)
		{
			auto data = GETDATA;

			KeyTypedEvent event((int)codepoint);
			data->EventCallback(event);
		}

		void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
		{
			auto data = GETDATA;

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data->EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data->EventCallback(event);
				break;
			}
			}
		}

		void OnScroll(GLFWwindow* window, double xOffset, double yOffset)
		{
			auto data = GETDATA;

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data->EventCallback(event);
		}

		void OnMouseMove(GLFWwindow* window, double x, double y)
		{
			auto data = GETDATA;

			MouseMovedEvent event((float)x, (float)y);
			data->EventCallback(event);
		}

#undef GETDATA
	}

	void WindowGLFWData::operator=(const WindowSpec& data)
	{
		Title = data.Title;
		OldWinWidth = Width = data.Width;
		OldWinHeight = Height = data.Height;
		Fullscreen = data.Fullscreen;
		VSync = data.VSync;
		WindowRenderer = data.WindowRenderer;
	}

	const std::string& WindowGLFW::GetTitle()
	{
		return m_windowData.Title;
	}

	void WindowGLFW::SetTitle(const char* newName)
	{
		m_windowData.Title = newName;
		glfwSetWindowTitle(m_nativeWindow, newName);
	}

	void WindowGLFW::SetFullscreen(bool fullscreen)
	{
		if (fullscreen == IsFullscreen())
		{
			return;
		}

		if (fullscreen)
		{
			// Backup window position and window size
			glfwGetWindowPos(m_nativeWindow, &m_windowData.OldWinPosX, &m_windowData.OldWinPosY);
			glfwGetWindowSize(m_nativeWindow, &m_windowData.OldWinWidth, &m_windowData.OldWinHeight);

			// Get resolution of current monitor
			GLFWmonitor* windowMonitor = GetWindowMonitor(m_nativeWindow);
			auto mode = glfwGetVideoMode(windowMonitor);
			m_windowData.RefreshRate = mode->refreshRate;

			// Go full screen
			glfwSetWindowMonitor(m_nativeWindow, windowMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			glfwSetWindowMonitor(m_nativeWindow, nullptr, m_windowData.OldWinPosX, m_windowData.OldWinPosY, m_windowData.OldWinWidth, m_windowData.OldWinHeight, GLFW_DONT_CARE);
		}
	}

	const bool WindowGLFW::IsFullscreen() const
	{
		return m_windowData.Fullscreen;
	}

	const bool WindowGLFW::ShouldClose() const
	{
		return m_windowData.ShouldClose;
	}

	const int WindowGLFW::GetWidth() const
	{
		return m_windowData.Width;
	}

	int const WindowGLFW::GetHeight() const
	{
		return m_windowData.Height;
	}

	bool const WindowGLFW::IsFocused() const
	{
		return m_windowData.Focused;
	}

	void WindowGLFW::DestroyWindow()
	{
		glfwDestroyWindow(m_nativeWindow);
	}

	WindowGLFW::WindowGLFW(const WindowSpec& initData) : Window(initData)
	{
		m_windowData = initData;

		// Create the window with glfw
		m_nativeWindow = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), (m_windowData.Fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
		if (m_nativeWindow == nullptr)
		{
			CL_CORE_ERROR("Unable to create a GLFW Window");
			return;
		}

		// Set it as active
		glfwMakeContextCurrent(m_nativeWindow);
		m_windowData.Focused = true;

		// Give OpenGL our user data
		glfwSetWindowUserPointer(m_nativeWindow, &m_windowData);

		// Subscribe to window events to propagate
		glfwSetWindowSizeCallback(m_nativeWindow, OnWindowResize);
		glfwSetWindowCloseCallback(m_nativeWindow, OnCloseWindow);
		glfwSetWindowPosCallback(m_nativeWindow, OnWindowMove);
		glfwSetWindowFocusCallback(m_nativeWindow, OnWindowFocus);
		glfwSetKeyCallback(m_nativeWindow, OnKey);
		glfwSetCharCallback(m_nativeWindow, OnChar);
		glfwSetMouseButtonCallback(m_nativeWindow, OnMouseButton);
		glfwSetScrollCallback(m_nativeWindow, OnScroll);
		glfwSetCursorPosCallback(m_nativeWindow, OnMouseMove);
	}

	void* const WindowGLFW::GetNative() const
	{
		return m_nativeWindow;
	}

	void const WindowGLFW::SwapBuffers() const
	{
		glfwSwapBuffers(m_nativeWindow);
	}

	void const WindowGLFW::PollEvents() const
	{
		glfwPollEvents();
	}

	Caramel::WindowRenderer WindowGLFW::GetWindowRendererType() const
	{
		return m_windowData.WindowRenderer;
	}

	void WindowGLFW::SetEventCallback(const EventCallback& callback)
	{
		m_windowData.EventCallback = callback;
	}

}