#include "Game.h"


Game::Game()
{

}

Game::~Game()
{
}

bool Game::OnCreate()
{
	std::string workingDir = Caramel::Utility::GetWorkingDir();

	Caramel::Scene* activeScene = Caramel::SceneManager::GetActiveScene();
	m_freeCam = FreeLookCam(activeScene);

	m_model = activeScene->CreateEntity("Model");
	auto& modelComp = m_model.AddComponent<Caramel::Components::Model>(workingDir + "resources/models/cerberus/Cerberus_LP.fbx");

	// Load and position model
	//model = Caramel::Model::LoadModel(workingDir + "resources/models/cerberus/Cerberus_LP.fbx");
	//modelTransform.Scale(glm::vec3(0.05f));

	// Setting up textures
	//shapeAlbedo = Caramel::Texture::CreateTexture(workingDir + "resources/models/cerberus/Textures/Cerberus_A.tga");
	//shapeNormal = Caramel::Texture::CreateTexture(workingDir + "resources/models/cerberus/Textures/Cerberus_N.tga");
	//shapeMetallic = Caramel::Texture::CreateTexture(workingDir + "resources/models/cerberus/Textures/Cerberus_M.tga");
	//shapeRoughness = Caramel::Texture::CreateTexture(workingDir + "resources/models/cerberus/Textures/Cerberus_R.tga");
	//shapeAO = Caramel::Texture::CreateTexture(workingDir + "resources/textures/PBR/bamboo/ao.png");


	return true;
}

void Game::Update(float deltaTime)
{
	m_freeCam.Update(deltaTime);
}

void Game::ImDraw()
{

}

void Game::Draw()
{
	Caramel::Utility::GetGLErrors();
}

void Game::Destroy()
{
}