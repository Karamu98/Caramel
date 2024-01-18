#pragma once
#include "Core/Window.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Caramel
{
	class Windows_Window_GLFW: public Window
	{
    public:
        Windows_Window_GLFW(const WindowProperties& properties);
		virtual ~Windows_Window_GLFW();

		void OnUpdate() override;

        virtual unsigned int GetWidth() const override;
        virtual unsigned int GetHeight() const override;
        virtual unsigned int GetRefreshRate() const override;
        virtual bool IsVSync() const override;

        virtual void SetEventCallback(const EventCallback& callback) override;
        virtual void SetVSync(bool enabled) override;
        virtual void SetRefreshRate(unsigned int frameRate) override;

    private:
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            unsigned int Framerate;

            EventCallback Callback;
        };

        WindowData m_data;
        GLFWwindow* m_window;

	};
}


