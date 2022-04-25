#pragma once

#include "Core/Window.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace Caramel::Implementation
{
	struct WindowGLFWData
	{
		std::string Title;
		unsigned int Width, Height;
		bool Fullscreen, VSync;
		bool Focused;
		bool ShouldClose;
		unsigned int RefreshRate;
		unsigned int WindowXPos, WindowYPos;
		WindowRenderer WindowRenderer;
		Window::EventCallback EventCallback;

		// Cached data for returning out of fullscreen
		int OldWinPosX, OldWinPosY, OldWinWidth, OldWinHeight;

	private:
		void operator=(const WindowSpec& data);
		friend class WindowGLFW;
	};

	class WindowGLFW : public Window
	{
	public:
		WindowGLFW(const WindowSpec& initData);
		WindowGLFW(WindowGLFW const&) = delete;
		void operator=(WindowGLFW const&) = delete;

		const std::string& GetTitle() override;
		void SetTitle(const char* newName) override;
		void SetFullscreen(bool isFull) override;
		const bool IsFullscreen() const override;
		const bool ShouldClose() const override;
		const int GetWidth() const override;
		int const GetHeight() const override;
		bool const IsFocused() const override;
		void DestroyWindow() override;
		void* const GetNative() const override;
		void const SwapBuffers() const override;
		void const PollEvents() const override;
		WindowRenderer GetWindowRendererType() const override;
		void SetEventCallback(const EventCallback& callback) override;

	private:
		WindowGLFWData m_windowData;
		GLFWwindow* m_nativeWindow;

	};
}
