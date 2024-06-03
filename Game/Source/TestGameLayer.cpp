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

    m_cam.ImGui();

    ImGui::End();
}

void TestGameLayer::OnUpdate(Caramel::Timestep ts)
{
    m_cam.OnUpdate(ts);
}

void TestGameLayer::Spawn()
{
    const std::vector<std::string> ModelPaths =
    {
        "Assets/Models/Ship/ship.obj",
        "Assets/Models/Sonic/sonic.obj",
        "Assets/Models/Callum/callu.obj",
        "Assets/Models/SpongeBoat/SPONGE1.obj",
        //"Assets/Models/Room/Room.fbx",
        //"Assets/Models/Sylvanas/Sylvanas (Mawsworn).dae"
    };

    m_models.clear();
    m_models.resize(m_countToSpawn);
    for (int i = 0; i < m_countToSpawn; ++i)
    {
        // Generate random 
        int randomIndex = std::rand() % ModelPaths.size();

        const std::string& randPath = ModelPaths[randomIndex];
        Caramel::Entity& currentEntity = m_models[i];
        currentEntity = m_scene.CreateEntity();
        currentEntity.RefPosition().x = m_seperation * i;

        auto& model = currentEntity.AddComponent<Caramel::COM_Model>();
        model.Model = Caramel::Model::GetOrLoad(randPath);
    }
}
