#include "clpch.h"
#include "Application.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Utilities.h"
#include "Render/Window.h"

namespace Caramel
{
	Application::Application() : m_isRunning(false), m_window(nullptr)
	{
	}

	bool Application::CreateApp(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
	{
		m_window = AppWindow::CreateWindow(a_name, a_width, a_height, a_bFullscreen);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings, with our main app window
		ImGui_ImplGlfw_InitForOpenGL(AppWindow::GetNative(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		return OnCreate();
	}

	void Application::Run(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
	{
		if (CreateApp(a_name, a_width, a_height, a_bFullscreen))
		{
			Utility::ResetTimer();
			m_isRunning = true;
			do
			{
				float deltaTime = Utility::TickTimer();

				// Start the Dear ImGui frame
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();


				Update(deltaTime);

				Draw();

				// Grab IO and make sure size is correct
				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize = ImVec2(AppWindow::GetWidth(), AppWindow::GetHeight());

				// Rendering
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Viewports rendering
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					GLFWwindow* backup_current_context = glfwGetCurrentContext();
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
					glfwMakeContextCurrent(backup_current_context);
				}


				glfwSwapBuffers(m_window->GetNative());
				glfwPollEvents();

			} while (m_isRunning == true && m_window->ShouldClose() == 0);

			Destroy();
		}
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		m_window->DestroyWindow();
		glfwTerminate();
	}
}

