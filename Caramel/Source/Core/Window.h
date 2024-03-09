#pragma once
#include "clpch.h"

#include "Core/Events/Event.h"

namespace Caramel
{
    struct WindowProperties
    {
        std::string Title = "Caramel Engine";
        unsigned int Width = 800;
        unsigned int Height = 600;
        bool VSync = false;
        unsigned int Framerate = 90;
        WindowRenderAPI RenderAPI = WindowRenderAPI::OpenGL;
    };

    class Window
    {
    public:
        using EventCallback = std::function<void(Event&)>;

        virtual ~Window() {};
        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual unsigned int GetRefreshRate() const = 0;
        virtual bool IsVSync() const = 0;

        virtual WindowRenderAPI GetRenderAPIType() = 0;
        class RenderAPI* GetRenderer() { return m_renderer; }

        template<typename T>
        T GetNativeWindow() 
        {
            return (T)GetNativeWindowImpl(typeid(T)); 
        };

        virtual void SetEventCallback(const EventCallback& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual void SetRefreshRate(unsigned int frameRate) = 0;

        static Window* Create(const WindowProperties& properties = WindowProperties());

    protected:
        virtual void* GetNativeWindowImpl(const type_info& typeInfo) = 0;

        class RenderAPI* m_renderer = nullptr;
    };
}
