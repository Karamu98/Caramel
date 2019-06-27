#pragma once

#include "Caramel/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Caramel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& a_props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_data.Width; }
		inline unsigned int GetHeight() const override { return m_data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& a_callback) override { m_data.EventCallback = a_callback; }
		void SetVSync(bool a_enabled);
		bool IsVSync() const;

		inline void* GetNativeWindow() const { return m_window; }
	private:
		virtual void Init(const WindowProps& a_props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_window;
		GLFWcursor* m_ImGuiMouseCursors[9] = { 0 };

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_data;
	};

}