#include "clpch.h"
#include "Core/Application.h"
#include "Core/ArgumentParser.h"
#include "Events/ApplicationEvent.h"
#include "Layers/ImGuiLayer.h"
#include "Core/Renderer.h"

Caramel::Application* Caramel::Application::s_application = nullptr;

Caramel::Application::Application()
{
    CL_CORE_ASSERT(!s_application);

    s_application = this;

    WindowProperties props{};
    std::string foundAPI;
    props.RenderAPI = WindowRenderAPI::OpenGL; // Default as its cross platform
    if (ArgumentParser::GetFlag("-gapi", &foundAPI))
    {
        if (foundAPI == "dx12")
        {
            props.RenderAPI = WindowRenderAPI::DirectX12;
        }
    }
    m_window.reset(Window::Create(props));
    m_window->SetEventCallback(BIND_EVENT_FN(&Application::OnEvent));
    RenderCommand::Init(m_window->GetRenderer());

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
        RenderCommand::Clear();
        Renderer::BeginScene();

        for (Layer* layer : m_layerStack)
        {
            layer->OnUpdate();
        }

        Renderer::EndScene();

        m_imGuiLayer->Begin();
        for (Layer* layer : m_layerStack)
        {
            layer->OnImGuiRender();
        }
        m_imGuiLayer->End();

        RenderCommand::Present();
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
