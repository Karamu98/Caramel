#pragma once

#include "Caramel/Core/Base.h"
#include "Caramel/Core/Window.h"
#include "Caramel/Core/LayerStack.h"

#include "Caramel/Core/Events/ApplicationEvent.h"

#include "Caramel/ImGui/ImGuiLayer.h"

namespace Caramel 
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		virtual void OnInit() {}
		virtual void OnShutdown() {}
		virtual void OnUpdate() {}

		virtual void OnEvent(Event& a_event);

		void PushLayer(Layer* a_layer); // Adds a layer to the LayerStack
		void PushOverlay(Layer* a_layer); // Adds an overlay to the LayerStack
		void RenderImGui();

		std::string OpenFile(const std::string& a_filter) const;

		inline Window& GetWindow() { return *m_window; }
		
		static inline Application& Get() { return *s_instance; }
	private:
		bool OnWindowResize(WindowResizeEvent& a_event);
		bool OnWindowClose(WindowCloseEvent& a_event);
	private:
		std::unique_ptr<Window> m_window;
		bool m_isRunning = true, m_isMinimised = false;
		LayerStack m_layerStack;
		ImGuiLayer* m_ImGuiLayer;

		static Application* s_instance;
	};

	// Implemented by CLIENT
	Application* CreateApplication();
}