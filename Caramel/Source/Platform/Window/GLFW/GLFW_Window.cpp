#include "clpch.h"
#include "GLFW_Window.h"
#include "Core/Log.h"
#include <Core/Events/ApplicationEvent.h>
#include <Core/Events/KeyEvents.h>
#include <Core/Events/MouseEvent.h>
#include <Core/RenderAPI/RenderAPI.h>

// TODO: IFDEF
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>


namespace Caramel
{
    static bool s_GLFWInitialised = false;

    static void GLFWErrorCallback(int error, const char* desc)
    {
        CL_CORE_ERROR("GLFW Error ({0}): {1}", error, desc);
    }

	GLFW_Window::GLFW_Window(const WindowProperties& properties)
	{
        m_data.Title = properties.Title;
        m_data.Width = properties.Width;
        m_data.Height = properties.Height;
        m_data.VSync = properties.VSync;
        m_data.Framerate = properties.Framerate;
        m_data.RenderAPI = properties.RenderAPI;

        if (!s_GLFWInitialised)
        {
            int success = glfwInit();
            CL_CORE_ASSERT(success, "Couldn't initialise GLFW");

            glfwSetErrorCallback(GLFWErrorCallback);

            s_GLFWInitialised = success == GLFW_TRUE;
        }

        CL_CORE_INFO("Creating new window named ({0})@{3}fps cap, w:{1} h:{2}, VSync: {4}", m_data.Title, m_data.Width, m_data.Height, m_data.Framerate, m_data.VSync);


        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_window = glfwCreateWindow(m_data.Width, m_data.Height, m_data.Title.c_str(), nullptr, nullptr);
        m_renderer = RenderAPI::Create(m_data.RenderAPI);
        if (!m_renderer)
        {
            CL_CORE_FATAL("Unable to create render api {}", (int)m_data.RenderAPI);
            return;
        }
        m_renderer->Initialise(this, &properties);
        //glfwMakeContextCurrent(m_window);

        SetRefreshRate(m_data.Framerate);
        glfwSetWindowUserPointer(m_window, &m_data);
        SetVSync(m_data.VSync);

        // Set callbacks
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowResizeEvent newEvent(w, h);
                data.Width = w;
                data.Height = h;
                data.Callback(newEvent);
                //ResizeRenderedBuffers(w, h);
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

	GLFW_Window::~GLFW_Window()
	{
        if (m_renderer)
        {
            m_renderer->Shutdown();
        }
		glfwDestroyWindow(m_window);
	}

	void GLFW_Window::OnUpdate()
	{
		glfwPollEvents();
        if (m_renderer)
        {
            m_renderer->RenderFrame();
        }
        else
        {
            glfwSwapBuffers(m_window);
        }
	}

	unsigned int GLFW_Window::GetWidth() const
	{
        return m_data.Width;
	}

	unsigned int GLFW_Window::GetHeight() const
	{
        return m_data.Height;
	}

	unsigned int GLFW_Window::GetRefreshRate() const
	{
        CL_CORE_NOT_IMPLEMENTED;
		return 0;
	}

	bool GLFW_Window::IsVSync() const
	{
		return m_data.VSync;
	}

	void GLFW_Window::SetEventCallback(const EventCallback& callback)
	{
		m_data.Callback = callback;
	}

	void GLFW_Window::SetVSync(bool enabled)
	{
        if (m_renderer)
        {
            m_renderer->SetVSync(enabled);
        }
        else
        {
            glfwSwapInterval(enabled == true ? 1 : 0);
        }
		m_data.VSync = enabled;
	}

	void GLFW_Window::SetRefreshRate(unsigned int frameRate)
	{
        if (m_renderer)
        {
            m_renderer->SetRefreshRate(frameRate);
        }
        else
        {
            CL_CORE_NOT_IMPLEMENTED;
        }
	}

    void* GLFW_Window::GetNativeWindowImpl(const type_info& typeInfo)
    {
        if (typeInfo == typeid(GLFWwindow*))
        {
            return m_window;
        }

        // TODO: IFDEF
        if (typeInfo == typeid(HWND))
        {
            return glfwGetWin32Window(m_window);
        }
        return nullptr;
    }

    WindowRenderAPI GLFW_Window::GetRenderAPIType()
    {
        return m_data.RenderAPI;
    }
}