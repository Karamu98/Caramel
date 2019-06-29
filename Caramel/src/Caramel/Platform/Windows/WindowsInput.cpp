#include "clpch.h"
#include "Caramel/Platform/Windows/WindowsInput.h"
#include "Caramel/Platform/Windows/WindowsWindow.h"

#include "Caramel/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Caramel 
{
	Input* Input::s_Instance = new WindowsInput;

	bool WindowsInput::IsKeyPressedImpl(int a_keycode)
	{
		// Test if the button is down and return it's state
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.GetNativeWindow()), a_keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int a_button)
	{
		// Grab the application instance to retrieve the window and cast it to Windows window
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());

		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.GetNativeWindow()), a_button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl()
	{
		// Grab our pair and return the x value
		auto [x, y] = GetMousePosImpl();
		return (float)x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		// Grab our pair and return the y value
		auto [x, y] = GetMousePosImpl();
		return (float)y;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		// Grab the application instance to retrieve the window and cast it to Windows window
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());

		// Create values for GLFW to change and pass them to glfw using our window
		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &xpos, &ypos);

		// Return our paired values
		return { xpos, ypos };
	}

}