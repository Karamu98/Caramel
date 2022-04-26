#include "clpch.h"
#include "Window.h"
#include "Imp/WindowGLFW.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>

namespace Caramel
{
	void Window::ShutdownContext(WindowRenderer renderContext)
	{
		switch (renderContext)
		{
		case Caramel::WindowRenderer::OpenGL:
		{
			glfwTerminate();
			break;
		}
		}
	}

	std::shared_ptr<Caramel::Window> Window::CreateApplicationWindow(const WindowSpec& data)
	{
		switch (data.Renderer)
		{
		case WindowRenderer::Vulkan:
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			return std::make_shared<Implementation::WindowGLFW>(data, WindowManager::GLFW, WindowRenderer::Vulkan, RendererFunctionLoader::Invalid);
			break;
		}
		case WindowRenderer::OpenGL:
		{
			if (!sm_glfwInitialised)
			{
				// Make sure glfw is initialised
				if (glfwInit() != GLFW_TRUE)
				{
					sm_glfwInitialised = false;
					CL_CORE_ERROR(false, "Unable to initialize GLFW");
					return nullptr;
				}
			}
			sm_glfwInitialised = true;

			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			auto newWindow = std::make_shared<Implementation::WindowGLFW>(data, WindowManager::GLFW, WindowRenderer::OpenGL, RendererFunctionLoader::GLAD);
			if (newWindow == nullptr)
			{
				return nullptr;
			}

			if (!sm_gladInitialised)
			{
				// Load glad
				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				{
					sm_glfwInitialised = false;
					sm_gladInitialised = false;
					glfwTerminate();
					return nullptr;
				}
				else
				{
					// Print OpenGL version to console
					CL_CORE_TRACE("\n=====\nOpenGl:\nVendor: {0}\nRenderer: {1}\nVersion: {2}\n=====\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
				}
			}
			sm_gladInitialised = true;

			return newWindow;
			break;
		}
		}

		return nullptr;
	}

	bool Window::sm_glfwInitialised = false;
	bool Window::sm_gladInitialised = false;
}