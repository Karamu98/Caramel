#include "TestGameLayer.h"

#include <imgui.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// SHADER
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 colour;
    layout (location = 2) in vec2 uv;

    out vec3 vColour;
    out vec2 vUV;

    uniform vec2 offset;

    void main()
    {
        vec2 newPos = position.xy + offset;
        gl_Position = vec4(newPos.xy, position.z, 1.0);
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
    m_testShader = Caramel::Shader::Create({ vertexShaderSource, fragmentShaderSource, {"offset"}});
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



    Assimp::Importer importer;
    const std::string modelPath = "Assets/Models/ship.obj";
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // Error handling
        CL_CORE_FATAL("ERROR::ASSIMP::{}", importer.GetErrorString());
        return;
    }
    else
    {
        CL_CORE_INFO("Loaded model ({0}):\n\tMeshes: {1}\n\tMaterials: {2}\n\tTextures: {5}\n\tAnimations: {3}\n\tSkeletons: {4}", modelPath, scene->mNumMeshes, scene->mNumMaterials, scene->mNumAnimations, scene->mNumSkeletons, scene->mNumTextures);
    }
}

void TestGameLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");

    ImGui::DragFloat("Spin speed", &m_spinSpeed);
    ImGui::DragFloat("Angle", &m_currrentAngle, 1.0f, 0.001f, 360.0f, "%.3f");
    ImGui::DragFloat("Radius", &m_radius, 0.01f, 0.001f);
    ImGui::Checkbox("Auto Spin", &m_spin);

    ImGui::End();
}

void TestGameLayer::SetPointOnCircle(float radius, float angleInDegrees)
{
    // Convert angle from degrees to radians
    float angleInRadians = angleInDegrees * 3.14159265358979323846 / 180.0f;

    // Calculate x and y using the polar coordinates to Cartesian coordinates conversion
    m_testOffset[0] = radius * cos(angleInRadians); // x-coordinate
    m_testOffset[1] = radius * sin(angleInRadians); // y-coordinate
}


void TestGameLayer::OnUpdate(Caramel::Timestep ts)
{
    m_testShader->Bind();
    if (m_spin)
    {
        m_currrentAngle += m_spinSpeed * ts;
        
    }
    SetPointOnCircle(m_radius, m_currrentAngle);
    m_testShader->SetValue("offset", Caramel::ShaderDataType::Float2, &m_testOffset);
    m_testTexture->Bind(0);
    Caramel::Renderer::Submit(m_testTriangle);
}
