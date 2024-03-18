#include "TestGameLayer.h"


/// SHADER
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 colour;
    layout (location = 2) in vec2 uv;

    out vec3 vColour;
    out vec2 vUV;

    void main()
    {
        gl_Position = vec4(position.x, position.y, position.z, 1.0);
        vColour = colour;
        vUV = uv;
    }
)glsl";

// Fragment shader source code
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 color;

    in vec3 vColour;
    in vec2 vUV;

    uniform sampler2D diffuse;

    void main()
    {
        vec4 texColour = texture(diffuse, vUV);
        color = texColour * vec4(vColour.xyz, 1.0f);
    }
)glsl";

void TestGameLayer::OnAttach()
{
    m_testShader = Caramel::Shader::Create({ vertexShaderSource, fragmentShaderSource });
    m_testTexture = Caramel::Texture::Create({ "Assets/wall.jpg", {Caramel::TextureType::T_2D} });
    Caramel::BufferLayout layout =
    {
        {Caramel::ShaderDataType::Float3, "position"},
        {Caramel::ShaderDataType::Float3, "colour"},
        {Caramel::ShaderDataType::Float2, "uv"}
    };

    {
        m_testTriangle = Caramel::VertexArray::Create();
        /// VERTEX
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,      0.0f, 0.0f, // Left  
             0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,      1.0f, 0.0f, // Right 
             0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f,      0.5f, 1.0f, // Top   
        };
        std::shared_ptr<Caramel::VertexBuffer> vbuffer = Caramel::VertexBuffer::Create({ vertices, sizeof(vertices) });
        vbuffer->SetLayout(layout);

        m_testTriangle->AddVertexBuffer(vbuffer);

        /// INDEX
        uint32_t indicies[] = { 0, 1, 2 };
        std::shared_ptr<Caramel::IndexBuffer> iBuffer = Caramel::IndexBuffer::Create({ indicies, sizeof(indicies) });
        m_testTriangle->SetIndexBuffer(iBuffer);
    }

    {
        float starVertices[] =
        {
            0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,    0.5f, 0.5f, // Center 0

            0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,    0.5f, 1.0f, // Top 1

            1.0f, 0.3f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f, // Right outer upper 2
            0.6f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,    1.0f, 0.0f, // Right outer lower 3

            0.3f, 0.3f, 0.0f,   0.0f, 1.0f, 0.0f,    0.8f, 1.0f, // Right inner upper 4
            0.3f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,   0.8f, 0.8f, // Right inner lower 5

            0.0f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.3f, // Bottom 6

            -1.0f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f  // Left outer upper 7 
            - 0.6f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Left outer lower 8

            -0.3f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f,   0.2f, 1.0f, // Left inner upper 9
            -0.3f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,  0.2f, 0.8f// Left inner lower 10
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

        m_testStar = Caramel::VertexArray::Create();
        std::shared_ptr<Caramel::VertexBuffer> vBuffer = Caramel::VertexBuffer::Create({ starVertices, sizeof(starVertices) });
        vBuffer->SetLayout(layout);
        m_testStar->AddVertexBuffer(vBuffer);

        std::shared_ptr<Caramel::IndexBuffer> iBuffer = Caramel::IndexBuffer::Create({ starIndices, sizeof(starIndices) });
        m_testStar->SetIndexBuffer(iBuffer);
    }
}

void TestGameLayer::OnUpdate()
{
    Caramel::Renderer::BeginScene();
    m_testShader->Bind();
    m_testTexture->Bind(0);
    Caramel::Renderer::Submit(m_testTriangle);
    Caramel::Renderer::EndScene();
}
