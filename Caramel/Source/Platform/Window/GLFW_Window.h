#pragma once
#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace Caramel
{
	class GLFW_Window : public Window
	{
    public:
        GLFW_Window(const WindowProperties& properties);
		virtual ~GLFW_Window();

		void OnUpdate() override;

        virtual unsigned int GetWidth() const override;
        virtual unsigned int GetHeight() const override;
        virtual unsigned int GetRefreshRate() const override;
        virtual bool IsVSync() const override;
        virtual void* GetNativeWindow() override;

        WindowRenderAPI GetRenderAPIType() override;

        virtual void SetEventCallback(const EventCallback& callback) override;
        virtual void SetVSync(bool enabled) override;
        virtual void SetRefreshRate(unsigned int frameRate) override;

        class RenderAPI* GetRenderer() { return m_renderer; }

    protected:
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            unsigned int Framerate;
            WindowRenderAPI RenderAPI;

            EventCallback Callback;
        };

        WindowData m_data;
        GLFWwindow* m_window;
        class RenderAPI* m_renderer = nullptr;

    };
}


