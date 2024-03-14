#pragma once
#include "Window.h"
#include "Layers/LayerStack.h"

namespace Caramel
{
    class ImGuiLayer;

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& event);

        void TrackLayer(Layer* layer);
        void TrackOverlay(Layer* overlay);

        inline Window* GetWindow() { return m_window.get(); };
        static inline Application* Get() { return s_application; }
        static inline WindowRenderAPI GetRenderAPI() { return s_application->GetWindow()->GetRenderAPIType(); }


    private:
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imGuiLayer;
        LayerStack m_layerStack;

        bool m_bIsRunning;

        static Application* s_application;
    };

}
