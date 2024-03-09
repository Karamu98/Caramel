#pragma once

#include <Core/ImGui/ImGuiWindowImpl.h>

#include <GLFW/glfw3.h>

namespace Caramel
{
	class ImGuiWindowImpl_GLFW : public ImGuiWindowImpl
	{
	public:

		// Inherited via ImGuiWindowImpl
		void Init(Window* window) override;
		void Shutdown() override;
		void NewFrame() override;
		void Render() override;

	private:
		GLFWwindow* m_window;
	};
}


