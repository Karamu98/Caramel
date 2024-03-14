#include "clpch.h"
#include "ImGuiRendererImpl_OpenGL.h"


#include <backends/imgui_impl_opengl3.h>

void Caramel::ImGuiRendererImpl_OpenGL::Init(RenderAPI* renderAPI)
{
	CL_CORE_ASSERT(ImGui_ImplOpenGL3_Init("#version 430 core"));
}

void Caramel::ImGuiRendererImpl_OpenGL::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void Caramel::ImGuiRendererImpl_OpenGL::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
}

void Caramel::ImGuiRendererImpl_OpenGL::Render(ImDrawData* drawData)
{
	ImGui_ImplOpenGL3_RenderDrawData(drawData);
}
