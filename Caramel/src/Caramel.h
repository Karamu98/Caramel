#pragma once

#include "Core\Application.h"
#include "Core\Utilities.h" 
#include "Core\Log.h"
#include "Core\Input.h"
#include "Scene\SceneManager.h"
#include "Scene\Scene.h"
#include "Scene\Components.h"
#include "Render\Texture.h"
#include "Render\Skybox.h"
#include "Render\Shader.h"
#include "Render\Window.h"
#include "Render\Framebuffer.h"
#include "Core\Resource\ModelResource.h"


#include "imgui.h"

namespace Caramel
{
	class Caramel
	{
	public:
		Caramel(Application* clientApplication);
		~Caramel();

		void Run(const char* windowName, int windowWidth, int windowHeight, bool isFullscreen);

	private:
		bool CreateApp(const char* windowName, int windowWidth, int windowHeight, bool isFullscreen);
		void ClientGUI();

	private:
		AppWindow* m_appWindow;
		Application* m_clientApplication;
		SceneManager m_sceneManager;
		//Input m_input;
		//Cursor m_cursor;
		//Screen m_screen;
		//AssetLoader m_assetLoader;
		//AssetManager m_assetManager
		//Log m_logger;
		//Utility m_utilities;
	};
}