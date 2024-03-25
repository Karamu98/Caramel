#include "TestGameLayer.h"

void TestGameLayer::OnAttach()
{
    m_cam.Camera.Position = { -5.0f, 4.5f, 4.7f };
    m_cam.Camera.Rotation = { 29.0f, 48.5f, 0.0f };
    m_cam.Camera.FOV = 60.0f;

    // Initialize random seed
    std::srand(std::time(0));

    Spawn();
}

void TestGameLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");

    ImGui::DragInt("Spawn count", &m_countToSpawn);
    ImGui::DragFloat("Spawn Seperation", &m_seperation);
    if (ImGui::Button("Spawn"))
    {
        Spawn();
    }

    ImGui::BeginListBox("Models");
    for (int i = 0; i < m_models.size(); ++i)
    {
        if (m_models[i])
        {
            ImGui::PushID(1000+i);
            m_models[i]->ImGui();
            ImGui::PopID();
        }
    }
    ImGui::EndListBox();

    m_cam.ImGui();

    ImGui::End();
}

void TestGameLayer::OnUpdate(Caramel::Timestep ts)
{
    m_cam.OnUpdate(ts);
    
    // Render temp
    {
        Caramel::Renderer::Submit(&m_cam.Camera);
        for (const auto& model : m_models)
        {
            if (model != nullptr)
            {
                model->Render();
            }
        }
    }
}

void TestGameLayer::Spawn()
{
    const std::vector<std::string> ModelPaths =
    {
        "Assets/Models/Ship/ship.obj",
        "Assets/Models/Sonic/sonic.obj",
        "Assets/Models/Callum/callu.obj",
        "Assets/Models/SpongeBoat/SPONGE1.obj",
    };

    m_models.clear();
    m_models.resize(m_countToSpawn);
    for (int i = 0; i < m_countToSpawn; ++i)
    {
        // Generate random index
        int randomIndex = std::rand() % ModelPaths.size();

        const std::string& randPath = ModelPaths[randomIndex];
        m_models[i] = std::make_shared<TestModel>();
        m_models[i]->Model = Caramel::Model::GetOrLoad(randPath);
        m_models[i]->Position.x = m_seperation * i;
    }
}
