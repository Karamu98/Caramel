#include "clpch.h"
#include "Core/Application.h"
#include "Core/ArgumentParser.h"

#include "Core/Log.h"
#include "Events/ApplicationEvent.h"

Caramel::Application* Caramel::Application::s_application = nullptr;

Caramel::Application::Application()
{
    CL_CORE_ASSERT(!s_application);

    s_application = this;

    WindowProperties props{};
    std::string foundAPI;
    if (ArgumentParser::GetFlag("-gapi", &foundAPI))
    {
        if (foundAPI == "dx12")
        {
            props.RenderAPI = WindowRenderAPI::DirectX12;
        }
    }
    m_window = std::unique_ptr<Window>(Window::Create(props));
    m_window->SetEventCallback(BIND_EVENT_FN(&Application::OnEvent));

    m_imGuiLayer = new ImGuiLayer();
    TrackOverlay(m_imGuiLayer);
}

Caramel::Application::~Application()
{
}

void Caramel::Application::Run()
{
    m_bIsRunning = true;

    while (m_bIsRunning)
    {
        for (Layer* layer : m_layerStack)
        {
            layer->OnUpdate();
        }

        m_imGuiLayer->Begin();
        for (Layer* layer : m_layerStack)
        {
            layer->OnImGuiRender();
        }
        m_imGuiLayer->End();

        m_window->OnUpdate();
    }
}

void Caramel::Application::OnEvent(Event& event)
{
    for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& closeEvent)
        {
                m_bIsRunning = false;
                return true;
        });

        (*--it)->OnEvent(event);
        if (event.Handled)
        {
            break;
        }
    }

    //CL_CORE_TRACE("{0}", event.ToString());
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
