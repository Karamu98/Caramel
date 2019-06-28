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
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());

		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.GetNativeWindow()), a_button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return (float)x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return (float)y;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		auto& window = static_cast<WindowsWindow&>(Application::Get().GetWindow());

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &xpos, &ypos);

		return { xpos, ypos };
	}

}