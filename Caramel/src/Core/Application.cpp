#include "clpch.h"
#include "Application.h"
#include "Utilities.h"

// TODO: Renderer abstraction
//#include <glad/glad.h>
//#include <glfw/glfw3.h>
//#include "examples/imgui_impl_glfw.h"
//#include "examples/imgui_impl_opengl3.h"

//#include <imgui.h>

namespace Caramel
{
	Application::Application() : m_appWindow(nullptr), m_isRunning(false)
	{
		// Prepare all engine tools
		SceneManager::Initialise(&m_sceneManager);
		m_logger.Init();
	}

	Application::~Application()
	{
		for (auto layer : m_layerStack)
		{
			layer->OnDetach();
		}

		// Close all engine tools
		SceneManager::Shutdown();
	}

	void Application::Run(const WindowSpec& data)
	{
		if (CreateApp(data))
		{
			// Reset timer and flag program as running
			Utility::ResetTimer();

			do
			{
				// Grab delta time and adjust by dilation
				float deltaTime = Utility::TickTimer();

				for (Layer* layer : m_layerStack)
				{
					layer->OnUpdate(deltaTime);
				}

				// Start the Dear ImGui frame
				//ImGui_ImplOpenGL3_NewFrame();
				//ImGui_ImplGlfw_NewFrame();
				//ImGui::NewFrame();

				// Renderer calls
				m_appWindow->SwapBuffers();
				m_appWindow->PollEvents();

			} while (m_isRunning);
		}

		// ImGui Cleanup
		//ImGui_ImplOpenGL3_Shutdown();
		//ImGui_ImplGlfw_Shutdown();
		//ImGui::DestroyContext();

		// OpenGL Cleanup
		m_appWindow->DestroyWindow();
		Window::ShutdownContext(m_appWindow->GetWindowRendererType());
	}


	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		// Listen for WindowResize and Close
		dispatcher.Dispatch<WindowResizeEvent>(CL_BIND(Application::OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(CL_BIND(Application::OnWindowClose));

		// Propagate events back to front (UI Layers drawn last etc)
		for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
			{
				break;
			}
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		int width = e.GetWidth(), height = e.GetHeight();
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_isRunning = false;
		return false;
	}

	bool Application::CreateApp(const WindowSpec& data)
	{
		m_appWindow = Window::CreateApplicationWindow(data);
		if (m_appWindow == nullptr)
		{
			CL_CORE_FATAL("Unable to create window.");
			return false;
		}
		m_input = Input::CreateInputManager({ m_appWindow->GetWindowManagerType() }, m_appWindow);
		m_appWindow->SetEventCallback(CL_BIND(Application::OnEvent));

		// Setup Dear ImGui context
		//IMGUI_CHECKVERSION();
		//ImGui::CreateContext();
		//ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		////io.ConfigViewportsNoAutoMerge = true;
		////io.ConfigViewportsNoTaskBarIcon = true;

		//// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		////ImGui::StyleColorsClassic();

		//// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		//ImGuiStyle& style = ImGui::GetStyle();
		//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		//{
		//	style.WindowRounding = 0.0f;
		//	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		//}

		// Initialise ImGUI
		//ImGui_ImplGlfw_InitForOpenGL(Window::GetNative(), true);
		//ImGui_ImplOpenGL3_Init("#version 330");

		m_isRunning = true;
		return true;
	}

	//void Application::ClientGUI()
	//{
	//	// Process the client draw commands
	//	m_clientApplication->ImDraw();

	//	// Grab IO and make sure size is correct
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.DisplaySize = ImVec2((float)m_appWindow->GetWidth(), (float)m_appWindow->GetHeight());

	//	// Rendering
	//	ImGui::Render();
	//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//	// Viewports rendering
	//	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//	{
	//		GLFWwindow* backup_current_context = glfwGetCurrentContext();
	//		ImGui::UpdatePlatformWindows();
	//		ImGui::RenderPlatformWindowsDefault();
	//		glfwMakeContextCurrent(backup_current_context);
	//	}
	//}

}

