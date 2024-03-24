#include "TestGameLayer.h"

const char* vertexShaderSource3D = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 v_Position;
    layout (location = 1) in vec3 v_Normal;
    layout (location = 2) in vec3 v_UV;
    
    uniform mat4 view;
    uniform mat4 projection;

    uniform mat4 transform;

    out vec3 vPos;

    void main()
    {
        vec4 worldPos = projection * view * transform * vec4(v_Position, 1.0);
        vPos = v_Position;
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
    m_test3DShader = Caramel::Shader::Create({ vertexShaderSource3D, fragmentShaderSource3D, {"transform", "view", "projection"}});

    m_testModel = Caramel::Model::GetOrLoad("Assets/Models/sonic.obj");
    m_testCamera.Position = { -10.0f, 5.0f, 0.0f };
    m_testCamera.Rotation = { 0.0f, 90.0f, 0.0f };
}

void TestGameLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");

    ImGui::DragFloat("Model spin speed", &m_modelSpinSpeed, 0.01f);
    ImGui::DragFloat3("Model position", glm::value_ptr(m_modelPosition));
    ImGui::DragFloat3("Model rotation", glm::value_ptr(m_modelRotation));
    ImGui::Spacing();

    ImGui::DrawWidget<Caramel::Camera>(&m_testCamera);

    ImGui::End();
}

void TestGameLayer::OnUpdate(Caramel::Timestep ts)
{
    m_curModelSpin += m_modelSpinSpeed * ts;

    glm::mat4 modelTransform = glm::mat4(1);
    modelTransform = glm::translate(modelTransform, m_modelPosition);

    m_test3DShader->Bind();
    m_test3DShader->SetValue("view", Caramel::ShaderDataType::Mat4, glm::value_ptr(m_testCamera.GetView()));
    m_test3DShader->SetValue("projection", Caramel::ShaderDataType::Mat4, glm::value_ptr(m_testCamera.GetProjection()));
    m_test3DShader->SetValue("transform", Caramel::ShaderDataType::Mat4, glm::value_ptr(modelTransform));

    Caramel::Renderer::Submit(m_testModel);
}