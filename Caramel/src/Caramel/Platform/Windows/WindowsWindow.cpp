#include "clpch.h"
#include <glad/glad.h>
#include "Caramel/Platform/Windows/WindowsWindow.h"

#include "Caramel/Core/Events/ApplicationEvent.h"
#include "Caramel/Core/Events/KeyEvent.h"
#include "Caramel/Core/Events/MouseEvent.h"

#include <imgui.h>

namespace Caramel 
{
	static void GLFWErrorCallback(int a_error, const char* a_description)
	{
		// Simply logging any error GLFW has to our logger
		CL_CORE_ERROR("GLFW Error ({0}): {1}", a_error, a_description);
	}
	
	static bool s_glfwInitialised = false;

	Window* Window::Create(const WindowProps& a_props)
	{
		return new WindowsWindow(a_props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& a_props)
	{
		Init(a_props);
	}

	WindowsWindow::~WindowsWindow()
	{
	}

	void WindowsWindow::Init(const WindowProps& a_props)
	{
		// Store our window data
		m_data.Title = a_props.Title;
		m_data.Width = a_props.Width;
		m_data.Height = a_props.Height;

		// Logging creation of window
		CL_CORE_INFO("Creating window {0} ({1}, {2})", a_props.Title, a_props.Width, a_props.Height);

		// Attempt to initialise GLFW
		if (!s_glfwInitialised)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			CL_CORE_ASSERT(success, "Could not intialise GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback); // Set the error callback

			s_glfwInitialised = true;
		}

		// Create and set our window
		m_window = glfwCreateWindow((int)a_props.Width, (int)a_props.Height, m_data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);

		// Attempt to initialise glad, assert on failure
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CL_CORE_ASSERT(status, "Failed to initialise Glad!");
		glfwSetWindowUserPointer(m_window, &m_data); // Setting the user pointer for later retrieval
		SetVSync(true);

		// Set resize callback
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* a_window, int a_width, int a_height)
		{
			// Get our user pointer back from GLFW
			auto& data = *((WindowData*)glfwGetWindowUserPointer(a_window));

			// Create the resize event, update and dispatch
			WindowResizeEvent event((unsigned int)a_width, (unsigned int)a_height);
			data.Width = a_width;
			data.Height = a_height;
			data.EventCallback(event);
		});

		// Set on window close callback	
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* a_window)
		{
			// Grab our user pointer from GLFW
			auto& data = *((WindowData*)glfwGetWindowUserPointer(a_window));

			// Create and dispatch the event
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		// Set the key callback
		glfwSetKeyCallback(m_window, [](GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
		{
			// Again, grab our user pointer
			auto& data = *((WindowData*)glfwGetWindowUserPointer(a_window));

			// Switch over our actions and get the correct one
			switch (a_action)
			{
				// Whatever case, create the event, pass the needed data and dispatch
				case GLFW_PRESS: // FIXME: GLFW Specific, needs changing
				{
					KeyPressedEvent event(a_key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(a_key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(a_key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		// Set our key typed callback
		glfwSetCharCallback(m_window, [](GLFWwindow* a_window, unsigned int a_codepoint)
		{
			// Grabbing the custom user pointer
			auto& data = *((WindowData*)glfwGetWindowUserPointer(a_window));

			// Create, set and dispatch event
			KeyTypedEvent event((int)a_codepoint);
			data.EventCallback(event);
		});

		// Set our mouse button callback
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* a_window, int a_button, int a_action, int a_mods)
		{
			// Grab the user pointer from GLFW
			auto& data = *((WindowData*)glfwGetWindowUserPointer(a_window));

			switch (a_action)
			{
				// Whatever case, create the event, pass the needed data and dispatch
				case GLFW_PRESS: // FIXME: GLFW Specific, needs changing
				{
					MouseButtonPressedEvent event(a_button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(a_button);
					data.EventCallback(event);
					break;
				}
			}
		});

		// Set our scrolling callback
		glfwSetScrollCallback(m_window, [](GLFWwindow* a_window, double a_xOffset, double a_yOffset)
		{
			// Grab the user pointer from GLFW
			auto& data = *((WindowData*)glfwGetWindowUserPointer(a_window));

			MouseScrolledEvent event((float)a_xOffset, (float)a_yOffset);
			data.EventCallback(event);
		});

		// Set our cursor position callback
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* a_window, double a_x, double a_y)
		{
			auto& data = *((WindowData*)glfwGetWindowUserPointer(a_window));

			MouseMovedEvent event((float)a_x, (float)a_y);
			data.EventCallback(event);
		});

		m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	}

	void WindowsWindow::Shutdown()
	{
		
	}

	void WindowsWindow::OnUpdate()
	{
		// Test our window events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(m_window);

		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		glfwSetCursor(m_window, m_ImGuiMouseCursors[imgui_cursor] ? m_ImGuiMouseCursors[imgui_cursor] : m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow]);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void WindowsWindow::SetVSync(bool a_enabled)
	{
		if (a_enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_data.VSync = a_enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_data.VSync;
	}

}
