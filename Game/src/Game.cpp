#include "Game.h"

class BaseGameLayer : public Caramel::Layer
{
public:

	void OnAttach() override
	{
		CL_TRACE("Attached layer!");
	}

	void OnDetach() override
	{
		CL_TRACE("Detached layer!");
	}

	void OnUpdate(float deltaTime) override
	{
		if (Caramel::Input::GetKey(Caramel::KeyCode::KEY_W))
		{
			CL_TRACE("Input key: W");
		}
	}

	void OnImGuiRender() override
	{
	}

	void OnEvent(Caramel::Event& event) override
	{
		Caramel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Caramel::KeyTypedEvent>(CL_BIND(BaseGameLayer::OnKeyTyped));
	}

private:
	bool OnKeyTyped(Caramel::KeyTypedEvent& event)
	{
		CL_TRACE("Event key typed: {0}", (char)event.GetKeyCode());
		return true;
	}
};

Game::Game()
{
	PushLayer(new BaseGameLayer());
	//std::string workingDir = Caramel::Utility::GetWorkingDir();

	//Caramel::Scene* activeScene = Caramel::SceneManager::GetActiveScene();
	//m_freeCam = FreeLookCam(activeScene);

	//m_model = activeScene->CreateEntity("Model");
	//auto& modelComp = m_model.AddComponent<Caramel::Components::Model>(workingDir + "resources/models/cerberus/Cerberus_LP.fbx");

	// Load and position model
	//model = Caramel::Model::LoadModel(workingDir + "resources/models/cerberus/Cerberus_LP.fbx");
	//modelTransform.Scale(glm::vec3(0.05f));

	// Setting up textures
	//shapeAlbedo = Caramel::Texture::CreateTexture(workingDir + "resources/models/cerberus/Textures/Cerberus_A.tga");
	//shapeNormal = Caramel::Texture::CreateTexture(workingDir + "resources/models/cerberus/Textures/Cerberus_N.tga");
	//shapeMetallic = Caramel::Texture::CreateTexture(workingDir + "resources/models/cerberus/Textures/Cerberus_M.tga");
	//shapeRoughness = Caramel::Texture::CreateTexture(workingDir + "resources/models/cerberus/Textures/Cerberus_R.tga");
	//shapeAO = Caramel::Texture::CreateTexture(workingDir + "resources/textures/PBR/bamboo/ao.png");
}

Game::~Game()
{
}