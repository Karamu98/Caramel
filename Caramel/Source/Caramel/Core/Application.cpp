#include "clpch.h"
#include "Caramel/Core/Application.h"
#include "Caramel/Core/ArgumentParser.h"
#include "Caramel/Core/Platform.h"
#include "Caramel/Core/Timestep.h"
#include "Caramel/Events/ApplicationEvent.h"
#include "Caramel/ImGui/ImGuiLayer.h"
#include "Caramel/Renderer/Renderer.h"

Caramel::Application* Caramel::Application::s_application = nullptr;

Caramel::Application::Application()
{
    CL_CORE_ASSERT(!s_application);

    s_application = this;

    WindowProperties props{};
    props.RenderAPI = WindowRenderAPI::OpenGL; // Default as its cross platform
    std::string foundAPI;
    if (ArgumentParser::GetFlag("-gapi", &foundAPI))
    {
        if (foundAPI == "dx12")
        {
            props.RenderAPI = WindowRenderAPI::DirectX12;
        }
    }
    m_window.reset(Window::Create(props));
    m_window->SetEventCallback(BIND_EVENT_FN(Application::OnWindowEvent));
    RenderCommand::Init(m_window->GetRenderer());

    SceneManager::Init(&m_sceneManager);

    m_imGuiRendererLayer = new ImGuiRendererLayer();
    m_imGuiRendererLayer->OnAttach();
}

Caramel::Application::~Application()
{
}

void Caramel::Application::Run()
{
    m_bIsRunning = true;

    while (m_bIsRunning)
    {
        float time = Platform::GetTime();
        Timestep timestep = time - m_lastFrameTime;
        m_lastFrameTime = time;

        RenderCommand::Clear();
        bool hasEditor = m_editorLayer != nullptr;
        if (hasEditor)
        {
            m_editorLayer->OnUpdate(timestep);
        }

        // TODO: Probo should block all these updates if editor layer determines it
        for (Layer* layer : m_layerStack)
        {
            layer->OnUpdate(timestep);
        }

        if (m_imGuiRendererLayer != nullptr)
        {
            m_imGuiRendererLayer->Begin();
            for (Layer* layer : m_layerStack)
            {
                layer->OnImGuiRender();
            }
            if (hasEditor)
            {
                m_editorLayer->OnImGuiRender();
            }
            m_imGuiRendererLayer->End();
        }

        RenderCommand::Present();
        m_window->OnUpdate();
    }
}

void Caramel::Application::OnWindowEvent(Event& event)
{
    if (m_editorLayer != nullptr)
    {
        m_editorLayer->OnEvent(event);
    }

    if (event.Handled)
    {
        return;
    }

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
