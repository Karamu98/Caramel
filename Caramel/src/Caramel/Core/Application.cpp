#include "clpch.h"
#include "Caramel/Core/Application.h"

#include "Caramel/Renderer/Renderer.h"
#include "Caramel/Renderer/Framebuffer.h"
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>

namespace Caramel 
{
// Macro to simplify std::bind's (Pass in the function)
#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		s_instance = this;

		// Create our "window" and set the callback
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer("ImGui");
		PushOverlay(m_ImGuiLayer);

		Renderer::Init();
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* a_layer)
	{
		// Adds the new layer to tracking and calls Init
		m_layerStack.PushLayer(a_layer);
		a_layer->OnAttach();
	}

	void Application::PushOverlay(Layer* a_layer)
	{
		// Adds the new overlay to tracking and calls Init
		m_layerStack.PushOverlay(a_layer);
		a_layer->OnAttach();
	}

	void Application::RenderImGui()
	{
		m_ImGuiLayer->StartFrame();

		ImGui::Begin("Renderer");
		auto& caps = RendererAPI::GetCapabilities();
		ImGui::Text("Vendor: %s", caps.Vendor.c_str());
		ImGui::Text("Renderer: %s", caps.Renderer.c_str());
		ImGui::Text("Version: %s", caps.Version.c_str());
		ImGui::End();

		for (Layer* layer : m_layerStack)
		{
			layer->OnImGuiRender();
		}

		m_ImGuiLayer->EndFrame();
	}

	void Application::Run()
	{
		// Initialise our application and the run if we're "running"
		OnInit();
		while (m_isRunning)
		{
			// If we're not minimised, update our layers
			if (!m_isMinimised)
			{
				for (Layer* layer : m_layerStack)
				{
					layer->OnUpdate();
				}

				// Render ImGui on render thread
				Application* app = this;
				CL_RENDER_1(app, { app->RenderImGui(); });

				Renderer::Get().WaitAndRender();
			}
			m_window->OnUpdate();
		}
		OnShutdown();
	}

	void Application::OnEvent(Event& a_event)
	{
		EventDispatcher dispatcher(a_event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// Loop backwards to handle events. Backwards so things like UI will receive events before game
		for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
		{
			// Try handle an event, if something handles it, break
			(*--it)->OnEvent(a_event);
			if (a_event.Handled)
			{
				break;
			}
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& a_event)
	{
		int width = a_event.GetWidth(), height = a_event.GetHeight();
		if (width == 0 || height == 0)
		{
			m_isMinimised = true;
			return false;
		}
		m_isMinimised = false;
		CL_RENDER_2(width, height, { glViewport(0, 0, width, height); });
		auto& fbs = FramebufferPool::GetGlobal()->GetAll();
		for (auto& fb : fbs)
			fb->Resize(width, height);
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& a_event)
	{
		m_isRunning = false;
		return true;
	}

	std::string Application::OpenFile(const std::string& a_filter) const
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_window->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "All\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

}