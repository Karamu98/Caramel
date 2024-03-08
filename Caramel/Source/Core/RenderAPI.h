#pragma once

// GLFW
#include <glfw/glfw3.h>

namespace Caramel
{
	class RenderAPI
	{
	protected:
		friend class GLFW_Window;

		virtual void Initialise(GLFWwindow* window, const struct WindowProperties* props) = 0;
		virtual void Shutdown() = 0;
		virtual void RenderFrame() = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual void SetRefreshRate(unsigned int frameRate) = 0;
	};
}


