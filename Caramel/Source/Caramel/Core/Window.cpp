#include "clpch.h"
#include "Window.h"

#include <Platform/Window/GLFW/GLFW_Window.h>

namespace Caramel
{
	Window* Window::Create(const WindowProperties& properties)
	{
		// Assume only glfw for now
		return new GLFW_Window(properties);
	}
}