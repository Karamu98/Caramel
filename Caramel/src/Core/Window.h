#pragma once

#include "clpch.h"

#include "Event/Event.h"
#include "Defines.h"

namespace Caramel
{
	struct WindowSpec
	{
		std::string Title = "Caramel Engine";
		unsigned int Width = 1280;
		unsigned int Height = 720;
		bool Fullscreen = false;
		bool VSync = false;
		WindowRenderer Renderer;
	};

	class Window
	{
	protected:
		Window(const WindowSpec& initData, WindowManager manager, WindowRenderer renderer, RendererFunctionLoader funcLoader) : 
			m_windowManager(manager), m_windowRenderer(renderer), m_rendererFuncLoader(funcLoader) {};
	public:
		using EventCallback = std::function<void(Event&)>;

		Window(Window const&) = delete;
		void operator=(Window const&) = delete;

		virtual const std::string& GetTitle() = 0;
		virtual void SetTitle(const char* newName) = 0;
		virtual void SetFullscreen(bool isFull) = 0;
		virtual const bool IsFullscreen() const = 0;
		virtual const bool ShouldClose() const = 0;
		virtual const int GetWidth() const = 0;
		virtual int const GetHeight() const = 0;
		virtual bool const IsFocused() const = 0;
		virtual void DestroyWindow() = 0;
		virtual void const SwapBuffers() const = 0;
		virtual void const PollEvents() const = 0;
		WindowManager GetWindowManagerType() const { return m_windowManager; };
		WindowRenderer GetWindowRendererType() const { return m_windowRenderer; };
		RendererFunctionLoader GetRenderFunctionLoaderType() const { return m_rendererFuncLoader; };
		virtual void SetEventCallback(const EventCallback& callback) = 0;
		virtual const void* GetNative() const = 0;

	public:
		static void ShutdownContext(WindowRenderer renderContext);
		static std::shared_ptr<Window> CreateApplicationWindow(const WindowSpec& data);
	private:
		static bool sm_glfwInitialised;
		static bool sm_gladInitialised;

	private:
		WindowRenderer m_windowRenderer;
		WindowManager m_windowManager;
		RendererFunctionLoader m_rendererFuncLoader;
	};
}