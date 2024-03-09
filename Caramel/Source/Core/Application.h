#pragma once
#include "Window.h"
#include "Layers/LayerStack.h"
#include "Layers/ImGuiLayer.h"

namespace Caramel
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& event);

        void TrackLayer(Layer* layer);
        void TrackOverlay(Layer* overlay);

        Window* GetWindow() { return m_window.get(); };

        template<typename T>
        static constexpr T* Get() { return (T*)s_application; }


    private:
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imGuiLayer;
        LayerStack m_layerStack;

        bool m_bIsRunning;


        static Application* s_application;
    };

}
