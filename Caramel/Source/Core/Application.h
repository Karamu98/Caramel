#pragma once
#include "Window.h"
#include "Layers/LayerStack.h"

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


    private:
        std::unique_ptr<Window> m_window;
        LayerStack m_layerStack;

        bool m_bIsRunning;
    };

}
