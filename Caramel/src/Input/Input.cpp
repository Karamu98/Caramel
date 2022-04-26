#include "clpch.h"
#include "Input.h"

#include "Core/Window.h"
#include "Impl/InputGLFW.h"


std::shared_ptr<Caramel::Input> Caramel::Input::CreateInputManager(WindowManager manager, std::shared_ptr<Window> inputWindow)
{
	switch (manager)
	{
	case WindowManager::GLFW:
	{
		return std::make_shared<InputGLFW>(inputWindow);
		break;
	}
	default:
		break;
	}
	return nullptr;
}

Caramel::Input* Caramel::Input::sm_instance = nullptr;
