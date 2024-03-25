#include "clpch.h"
#include "GLFW_Input.h"

#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace Caramel
{
	bool GLFW_Input::IsKeyPressedImpl(KeyCode code)
	{
		auto GLFWWindow = Application::Get()->GetWindow()->GetNativeWindow<GLFWwindow*>();
		int state = glfwGetKey(GLFWWindow, (int)code);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool GLFW_Input::IsMouseButtonPressedImpl(int button)
	{
		auto GLFWWindow = Application::Get()->GetWindow()->GetNativeWindow<GLFWwindow*>();
		int state = glfwGetMouseButton(GLFWWindow, button);
		return state == GLFW_PRESS;
	}

	glm::vec2 GLFW_Input::GetMousePositionImpl()
	{
		auto GLFWWindow = Application::Get()->GetWindow()->GetNativeWindow<GLFWwindow*>();
		double xpos, ypos;
		glfwGetCursorPos(GLFWWindow, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	void GLFW_Input::SetMousePositionImpl(glm::vec2 pos)
	{
		auto GLFWWindow = Application::Get()->GetWindow()->GetNativeWindow<GLFWwindow*>();
		glfwSetCursorPos(GLFWWindow, pos.x, pos.y);
	}
}

