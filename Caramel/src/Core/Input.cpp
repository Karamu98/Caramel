#include "clpch.h"
#include "Input.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"


bool Caramel::Input::GetKeyDown(char character)
{
	return glfwGetKey(glfwGetCurrentContext(), character) == GLFW_PRESS;
}

bool Caramel::Input::GetKeyUp(char character)
{
	return glfwGetKey(glfwGetCurrentContext(), character) == GLFW_RELEASE;
}

bool Caramel::Input::GetKey(char character)
{
	int state = glfwGetKey(glfwGetCurrentContext(), character);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Caramel::Input::GetMouseButton(int button)
{
	int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Caramel::Input::GetMouseButtonDown(int button)
{
	return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;
}

bool Caramel::Input::GetMouseButtonUp(int button)
{
	return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_RELEASE;
}