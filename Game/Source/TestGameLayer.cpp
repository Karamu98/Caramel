#include "TestGameLayer.h"

#include <imgui.h>
#include <assimp/BaseImporter.h>

const char* vertexShaderSource3D = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 v_Position;
    
    uniform mat4 viewMat;
    uniform mat4 camTrans;

    uniform mat4 transform;

    out vec3 vPos;

    void main()
    {
        vec4 worldPos = transform * vec4(v_Position.xyz, 1.0);
        vPos = v_Position.xyz;
        gl_Position = worldPos;
    }
)glsl";

// Fragment shader source code
const char* fragmentShaderSource3D = R"glsl(
    #version 330 core
    out vec4 color;

    in vec3 vPos;

    void main()
    {
        color = vec4(vPos.xyz, 1.0f);
    }
)glsl";



void TestGameLayer::OnAttach()
{
    m_test3DShader = Caramel::Shader::Create({ vertexShaderSource3D, fragmentShaderSource3D, {"transform"} });

    m_testModel = Caramel::Model::GetOrLoad("Assets/Models/sonic.obj");
}

void TestGameLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");

    ImGui::DragFloat("Model spin speed", &m_modelSpinSpeed, 0.01f);
    ImGui::Spacing();
    ImGui::DragFloat("FOV", &m_fov, 0.1f);
    ImGui::DragFloat("Near plane", &m_nearPlane, 0.01f);
    ImGui::DragFloat("Far plane", &m_farPlane, 0.01f);

    ImGui::End();
}

double degreesToRadians(double degrees)
{
    return degrees * 3.14159265358979323846 / 180.0;
}

void TestGameLayer::OnUpdate(Caramel::Timestep ts)
{
    m_curModelSpin += m_modelSpinSpeed * ts;

    float tanHalfFOV = tanf(degreesToRadians(m_fov / 2));
    float d = 1 / tanHalfFOV;

    auto wind = Caramel::Application::Get()->GetWindow();
    float ar = (float)wind->GetWidth() / (float)wind->GetHeight();

    float nearFarRange = m_nearPlane - m_farPlane;

    float projA = (-m_farPlane - m_nearPlane) / nearFarRange;
    float projB = 2.0f * m_farPlane * m_nearPlane / nearFarRange;

    aiMatrix4x4 rot
    {
        cosf(m_curModelSpin), 0.0f, -sinf(m_curModelSpin), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(m_curModelSpin), 0.0f, cosf(m_curModelSpin), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    aiMatrix4x4 trans
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -5.0f,
        0.0f, 0.0f, 1.0f, 10.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    aiMatrix4x4 proj
    {
        d/ar, 0.0f, 0.0f, 0.0f,
        0.0f, d, 0.0f, 0.0f,
        0.0f, 0.0f, projA, projB,
        0.0f, 0.0f, 1.0f, 0.0f,
    };

    aiMatrix4x4 finalMat = proj * trans * rot;



    m_test3DShader->Bind();
    m_test3DShader->SetValue("transform", Caramel::ShaderDataType::Mat4, &finalMat);
    //Caramel::Renderer::Submit(m_testCube);
    Caramel::Renderer::Submit(m_testModel);
}
