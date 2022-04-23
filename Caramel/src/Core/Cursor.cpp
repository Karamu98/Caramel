#include "clpch.h"
#include "Cursor.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"


glm::tvec2<double> Caramel::Cursor::GetPosition()
{
	glm::tvec2<double> pos;
	glfwGetCursorPos(glfwGetCurrentContext(), &pos.x, &pos.y);
	return pos;
}