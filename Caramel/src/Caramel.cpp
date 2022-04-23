#include "clpch.h"
#include "Caramel.h"

// TODO: Renderer abstraction
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

Caramel::Caramel::Caramel(Application* clientApplication) : m_clientApplication(clientApplication), m_appWindow(nullptr)
{
	// Prepare all engine tools
	SceneManager::Initialise(&m_sceneManager);
	Log::Init();
}

Caramel::Caramel::~Caramel()
{
	SceneManager::Shutdown();
}

void Caramel::Caramel::Run(const char* windowName, int windowWidth, int windowHeight, bool isFullscreen)
{
	if (CreateApp(windowName, windowWidth, windowHeight, isFullscreen))
	{
		// Reset timer and flag program as running
		Utility::ResetTimer();

		// Default opengl
		glEnable(GL_DEPTH_TEST);

		// Program loop
		do
		{
			// Grab delta time and adjust by dilation
			float deltaTime = Utility::TickTimer();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Application events
			m_clientApplication->Update(deltaTime);
			m_clientApplication->Draw();
			ClientGUI();

			// GL display calls
			glfwSwapBuffers(m_appWindow->GetNative());
			glfwPollEvents();

		} while (m_appWindow->ShouldClose() == 0);

		// Client destroy call
		m_clientApplication->Destroy();
	}

	// ImGui Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// OpenGL Cleanup
	m_appWindow->DestroyWindow();
	glfwTerminate();
}

bool Caramel::Caramel::CreateApp(const char* windowName, int windowWidth, int windowHeight, bool isFullscreen)
{
	m_appWindow = AppWindow::CreateGLWindow(windowName, windowWidth, windowHeight, isFullscreen);

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

	return m_clientApplication->OnCreate();
}

void Caramel::Caramel::ClientGUI()
{
	// Process the client draw commands
	m_clientApplication->ImDraw();

	// Grab IO and make sure size is correct
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)AppWindow::GetWidth(), (float)AppWindow::GetHeight());

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
}