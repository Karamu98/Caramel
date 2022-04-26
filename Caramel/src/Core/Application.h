#pragma once
#include "Event/ApplicationEvent.h"
#include "Core/Window.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Scene/SceneManager.h"
#include "Input/Input.h"


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
		SceneManager m_sceneManager;
		LayerStack m_layerStack;
		std::shared_ptr<Input> m_input;
		bool m_isRunning = false;
		Log m_logger;
		//Screen m_screen;
		//AssetLoader m_assetLoader;
		//AssetManager m_assetManager
		//Utility m_utilities;
	};
}