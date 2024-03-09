#include "clpch.h"
#include "ImGuiWindowImpl.h"

#include <Core/Application.h>


#include <Platform/ImGuiImpl/GLFW/ImGuiWindowImpl_GLFW.h>

namespace Caramel
{
	ImGuiWindowImpl* Caramel::ImGuiWindowImpl::Create()
	{
		return new ImGuiWindowImpl_GLFW();
	}
}

