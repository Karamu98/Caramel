#include "clpch.h"
#include "Windows_Window.h"
#include "Core/Log.h"
#include <Core/Events/ApplicationEvent.h>
#include <Core/Events/KeyEvents.h>
#include <Core/Events/MouseEvent.h>

namespace Caramel
{
	static bool s_GLFWInitialised = false;

	static void GLFWErrorCallback(int error, const char* desc)
	{
		CL_CORE_ERROR("GLFW Error ({0}): {1}", error, desc);
	}

	Window* Window::Create(const WindowProperties& properties)
	{
		return new Windows_Window_GLFW(properties);
	}

	Windows_Window_GLFW::Windows_Window_GLFW(const WindowProperties& properties)
	{
		m_data.Title = properties.Title;
		m_data.Width = properties.Width;
		m_data.Height = properties.Height;
		m_data.VSync = properties.VSync;
		m_data.Framerate = properties.Framerate;

		if (!s_GLFWInitialised)
		{
			int success = glfwInit();
			CL_CORE_ASSERT(success, "Couldn't initialise GLFW");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialised = success == GLFW_TRUE;
		}

		CL_CORE_INFO("Creating new window named ({0})@{3}fps cap, w:{1} h:{2}, VSync: {4}", m_data.Title, m_data.Width, m_data.Height, m_data.Framerate, m_data.VSync);

		//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_window = glfwCreateWindow(m_data.Width, m_data.Height, m_data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(m_data.VSync);
		SetRefreshRate(m_data.Framerate);

		// Set callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent newEvent(w, h);
				data.Width = w;
				data.Height = h;
				data.Callback(newEvent);
			});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent newEvent;
				data.Callback(newEvent);

			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressEvent newEvent(key, 0);
						data.Callback(newEvent);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleaseEvent newEvent(key);
						data.Callback(newEvent);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressEvent newEvent(key, 1);
						data.Callback(newEvent);
						break;
					}
				default:
					break;
				}
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressEvent newEvent(button);
						data.Callback(newEvent);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleaseEvent newEvent(button);
						data.Callback(newEvent);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOff, double yOff)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrollEvent newEvent((float)xOff, (float)yOff);
				data.Callback(newEvent);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMoveEvent newEvent((float)xPos, (float)yPos);
				data.Callback(newEvent);
			});
	}

	Windows_Window_GLFW::~Windows_Window_GLFW()
	{
		glfwDestroyWindow(m_window);
	}

	void Windows_Window_GLFW::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	unsigned int Windows_Window_GLFW::GetWidth() const
	{
		return 0;
	}

	unsigned int Windows_Window_GLFW::GetHeight() const
	{
		return 0;
	}

	unsigned int Windows_Window_GLFW::GetRefreshRate() const
	{
		return 0;
	}

	bool Windows_Window_GLFW::IsVSync() const
	{
		return m_data.VSync;
	}

	void Windows_Window_GLFW::SetEventCallback(const EventCallback& callback)
	{
		m_data.Callback = callback;
	}

	void Windows_Window_GLFW::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled == true ? 1 : 0);
		m_data.VSync = enabled;
	}

	void Windows_Window_GLFW::SetRefreshRate(unsigned int frameRate)
	{
	}
}