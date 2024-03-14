#pragma once
#include "Core/Window.h"

struct GLFWwindow;

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

        WindowRenderAPI GetRenderAPIType() override;

        virtual void SetEventCallback(const EventCallback& callback) override;
        virtual void SetVSync(bool enabled) override;
        virtual void SetRefreshRate(unsigned int frameRate) override;        

        

    protected:
        virtual void* GetNativeWindowImpl(const type_info& typeInfo) override;
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            unsigned int Framerate;
            WindowRenderAPI RenderAPI;

            EventCallback Callback;
        };

        GLFWwindow* m_window;
        WindowData m_data;
    };
}


