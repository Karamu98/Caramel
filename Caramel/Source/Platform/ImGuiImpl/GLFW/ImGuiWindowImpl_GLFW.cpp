#include "clpch.h"
#include "ImGuiWindowImpl_GLFW.h"


#include <backends/imgui_impl_glfw.h>

#include <Core/Window.h>

namespace Caramel
{
	void ImGuiWindowImpl_GLFW::Init(Window* window)
	{
		m_window = window->GetNativeWindow<GLFWwindow*>();
		ImGui_ImplGlfw_InitForOther(m_window, true);
	}

	void ImGuiWindowImpl_GLFW::Shutdown()
	{
		ImGui_ImplGlfw_Shutdown();
	}

	void ImGuiWindowImpl_GLFW::NewFrame()
	{
		ImGui_ImplGlfw_NewFrame();
	}

	void ImGuiWindowImpl_GLFW::Render()
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}

