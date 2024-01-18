#include "clpch.h"
#include "Core/Application.h"

#include "Core/Log.h"

Caramel::Application::Application()
{
    m_window = std::unique_ptr<Window>(Window::Create());
    m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

Caramel::Application::~Application()
{
}

void Caramel::Application::Run()
{
    while (true)
    {
        for (Layer* layer : m_layerStack)
        {
            layer->OnUpdate();
        }

        m_window->OnUpdate();
    }
}

void Caramel::Application::OnEvent(Event& event)
{
    for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
    {
        (*--it)->OnEvent(event);
        if (event.Handled)
        {
            break;
        }
    }

    CL_CORE_TRACE("{0}", event.ToString());

}

void Caramel::Application::TrackLayer(Layer* layer)
{
    m_layerStack.TrackLayer(layer);
    layer->OnAttach();
}

void Caramel::Application::TrackOverlay(Layer* overlay)
{
    m_layerStack.TrackOverlay(overlay);
    overlay->OnAttach();
}
