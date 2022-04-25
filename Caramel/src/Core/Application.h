#pragma once
#include "Event/ApplicationEvent.h"
#include "Core/Window.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Scene/SceneManager.h"


namespace Caramel
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run(const WindowSpec& data);

		void PushLayer(Layer* newLayer);
		void PushOverlay(Layer* newOverlay);
		virtual void OnEvent(Event& event);

	private:
		bool CreateApp(const WindowSpec& data);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::shared_ptr<Window> m_appWindow;
		Application* m_clientApplication;
		SceneManager m_sceneManager;
		LayerStack m_layerStack;
		bool m_isRunning = false;
		//Input m_input;
		//Cursor m_cursor;
		//Screen m_screen;
		//AssetLoader m_assetLoader;
		//AssetManager m_assetManager
		//Log m_logger;
		//Utility m_utilities;
	};
}