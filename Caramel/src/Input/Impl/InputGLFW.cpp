#include "clpch.h"
#include "InputGLFW.h"

#include "GLFW/glfw3.h"


Caramel::InputGLFW::InputGLFW(std::shared_ptr<Window> inputWindow) : m_inputWindow(inputWindow)
{
	sm_instance = this;
}

bool Caramel::InputGLFW::GetKeyDownImpl(KeyCode keyCode)
{
	return glfwGetKey((GLFWwindow*)m_inputWindow->GetNative(), (int)keyCode) == GLFW_PRESS;
}

bool Caramel::InputGLFW::GetKeyUpImpl(KeyCode keyCode)
{
	return glfwGetKey((GLFWwindow*)m_inputWindow->GetNative(), (int)keyCode) == GLFW_RELEASE;
}

bool Caramel::InputGLFW::GetKeyImpl(KeyCode keyCode)
{
	int state = glfwGetKey((GLFWwindow*)m_inputWindow->GetNative(), (int)keyCode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Caramel::InputGLFW::GetMouseButtonImpl(MouseButton button)
{
	int state = glfwGetMouseButton((GLFWwindow*)m_inputWindow->GetNative(), (int)button);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Caramel::InputGLFW::GetMouseButtonDownImpl(MouseButton button)
{
	return glfwGetMouseButton((GLFWwindow*)m_inputWindow->GetNative(), (int)button) == GLFW_PRESS;
}

bool Caramel::InputGLFW::GetMouseButtonUpImpl(MouseButton button)
{
	return glfwGetMouseButton((GLFWwindow*)m_inputWindow->GetNative(), (int)button) == GLFW_RELEASE;
}

std::pair<float, float> Caramel::InputGLFW::GetMousePositionImpl()
{
	double x, y;
	glfwGetCursorPos((GLFWwindow*)m_inputWindow->GetNative(), &x, & y);
	return { x, y };
}