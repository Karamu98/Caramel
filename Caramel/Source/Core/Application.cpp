#include "clpch.h"
#include "Core/Application.h"
#include "Core/ArgumentParser.h"
#include "Events/ApplicationEvent.h"
#include "Layers/ImGuiLayer.h"


/////////////////////////////////

#include <Core/Renderer.h>
#include <Core/RenderAPI/Buffer.h>
#include <Core/RenderAPI/Shader.h>


    /// SHADER
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 colour;

    out vec3 vColour;

    void main()
    {
        gl_Position = vec4(position.x, position.y, position.z, 1.0);
        vColour = colour;
    }
)glsl";

// Fragment shader source code
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 color;

    in vec3 vColour;

    void main()
    {
        color = vec4(vColour.xyz, 1.0f);
    }
)glsl";

namespace Caramel
{
    std::shared_ptr<Shader> DEBUG_testShader;
    std::shared_ptr<VertexArray> DEBUG_testArray;
    std::shared_ptr<VertexArray> DEBUG_testStar;

    void DEBUG_Init()
    {
        DEBUG_testShader = Shader::Create({ vertexShaderSource, fragmentShaderSource });
        BufferLayout layout =
        {
            {ShaderDataType::Float3, "position"},
            {ShaderDataType::Float3, "colour"}
        };

        {
            DEBUG_testArray = VertexArray::Create();
            /// VERTEX
            float vertices[] = {
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Left  
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Right 
                 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Top   
            };
            std::shared_ptr<VertexBuffer> vbuffer = VertexBuffer::Create({ vertices, sizeof(vertices) });
            vbuffer->SetLayout(layout);

            DEBUG_testArray->AddVertexBuffer(vbuffer);

            /// INDEX
            uint32_t indicies[] = { 0, 1, 2 };
            std::shared_ptr<IndexBuffer> iBuffer = IndexBuffer::Create({ indicies, sizeof(indicies) });
            DEBUG_testArray->SetIndexBuffer(iBuffer);
        }

        {
            float starVertices[] = 
            {
                0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, // Center 0

                0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f, // Top 1

                1.0f, 0.3f, 0.0f,   0.0f, 1.0f, 0.0f, // Right outer upper 2
                0.6f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Right outer lower 3

                0.3f, 0.3f, 0.0f,   0.0f, 1.0f, 0.0f, // Right inner upper 4
                0.3f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f, // Right inner lower 5

                0.0f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f, // Bottom 6

                -1.0f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f, // Left outer upper 7 
                -0.6f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Left outer lower 8

                -0.3f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f, // Left inner upper 9
                -0.3f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f, // Left inner lower 10
            };

            // Indices array
            unsigned int starIndices[] = 
            {
                0, 1, 4,
                0, 4, 2,
                0, 2, 5,
                0, 5, 3,
                0, 3, 6,

                0, 6, 8,
                0, 8, 10,
                0, 10, 7,
                0, 7, 9,
                0, 9, 1
            };

            DEBUG_testStar = VertexArray::Create();
            std::shared_ptr<VertexBuffer> vBuffer = VertexBuffer::Create({ starVertices, sizeof(starVertices) });
            vBuffer->SetLayout(layout);
            DEBUG_testStar->AddVertexBuffer(vBuffer);

            std::shared_ptr<IndexBuffer> iBuffer = IndexBuffer::Create({ starIndices, sizeof(starIndices) });
            DEBUG_testStar->SetIndexBuffer(iBuffer);
        }
    }

    void DEBUG_Draw()
    {
        DEBUG_testShader->Bind();
        Renderer::Submit(DEBUG_testStar);
        //Renderer::Submit(DEBUG_testArray);
    }
}


/////////////////////////////////

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


    /////////////////////////////////////
    DEBUG_Init();
    /////////////////////////////////////
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

        RenderCommand::Clear();

        Renderer::BeginScene();
        //////////////////////////
        DEBUG_Draw();
        //////////////////////////
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
