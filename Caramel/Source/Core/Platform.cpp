#include "clpch.h"
#include "Platform.h"

#include <GLFW/glfw3.h>

namespace Caramel
{
	float Platform::GetTime()
	{
		return glfwGetTime();
	}
}

