#include "MyApplication.h"
#include "glad/glad.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "Component.h"
#include "Log.h"
#include "MeshFilter.h"
#include "Camera.h"
#include "Entity.h"
#include "Renderer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Cube.h"


MyApplication::MyApplication() : m_showUI(true)
{

}

MyApplication::~MyApplication()
{
}

bool MyApplication::onCreate()
{
	// initialise the Gizmos helper class
	Gizmos::create();

	// Initialise logging system
	Log::Init();
	CL_CORE_INFO("Initialised.");

	CL_CORE_INFO("\n=====\nOpenGL Information:\nVendor: {0}\nRenderer: {1}\nVersion: {2}\n=====", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));

	// Initialise Renderer
	m_renderer.Init(RenderingMode::DEFERRED);

#pragma region Adding scene entities

	// Add the default entity.
	Entity* newEditor = new Entity(&m_scene);
	newEditor->SetName("Editor");
	Camera* newCam = new Camera(newEditor);
	m_scene.m_activeCamera = newCam;
	newEditor->GetTransform()->SetPosition(glm::vec3(-1.5, 4, 17));
	SpotLight* camFlash = new SpotLight(newEditor);

	// Add a directional light and point light
	Entity* lightHolder = new Entity(&m_scene);
	lightHolder->SetName("Directional");
	lightHolder->GetTransform()->SetPosition(glm::vec3(5, 5, 5));
	DirectionalLight* sceneLight = new DirectionalLight(lightHolder);
	sceneLight->SetDirection(glm::vec3(0.55f, -0.35f, -0.7f));
	sceneLight->SetDiffuse(glm::vec3(1.0f, 0.737f, 0.36f));
	sceneLight->SetSpecular(glm::vec3(1.0f, 0.7f, 0.0f));
	Entity* pointHolder = new Entity(&m_scene);
	pointHolder->SetName("Point");
	PointLight* pointLight = new PointLight(pointHolder);
	pointHolder->GetTransform()->SetPosition(glm::vec3(0.1f, 4.7f, 10.9f));
	pointLight->SetDiffuse(glm::vec3(0.90f, 0.0f, 0.90f));
	pointLight->SetSpecular(glm::vec3(1.0f, 0.0f, 1.0f));

	// Add the ruins
	Entity* ruinsEntity = new Entity(&m_scene); // Create the entity
	ruinsEntity->SetName("Ruins");
	ruinsEntity->GetTransform()->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	ruinsEntity->GetTransform()->SetPosition(glm::vec3(0, 1, 0));
	MeshFilter* ruins = new MeshFilter(ruinsEntity, SOLID | SHADCAST); // Create its mesh filter
	ruins->LoadModel("models/Ruins/Ruins.obj"); // Load the model

	Entity* nanosuitEntity = new Entity(&m_scene); // Create the entity
	nanosuitEntity->SetName("Nanosuit");
	nanosuitEntity->GetTransform()->SetScale(glm::vec3(0.2f));
	nanosuitEntity->GetTransform()->SetPosition(glm::vec3(0, 1.5f, 10));
	MeshFilter* nanoMesh = new MeshFilter(nanosuitEntity, SOLID | SHADCAST); // Create its mesh filter
	nanoMesh->LoadModel("models/nanosuit/nanosuit.obj"); // Load the model

	// Add the squid
	Entity* squidEntity = new Entity(&m_scene); // Create the entity
	squidEntity->SetName("Squid");
	squidEntity->GetTransform()->SetScale(glm::vec3(0.01f));
	squidEntity->GetTransform()->SetPosition(glm::vec3(-2.5f, 3.3f, 10));
	MeshFilter* squidMesh = new MeshFilter(squidEntity, SOLID | SHADCAST); // Create its mesh filter
	squidMesh->LoadModel("models/AlexBloor/Alex.fbx"); // Load the model

	// Add the specular cube
	Entity* boxEntity = new Entity(&m_scene); // Create the entity
	boxEntity->SetName("Box");
	boxEntity->GetTransform()->SetScale(glm::vec3(0.01f));
	boxEntity->GetTransform()->SetPosition(glm::vec3(-5.0f, 2.0f, 10));
	MeshFilter * boxMesh = new MeshFilter(boxEntity, SOLID | SHADCAST); // Create its mesh filter
	boxMesh->LoadModel("models/DragonBox/box.fbx"); // Load the model

	// Add the waves
	Entity* wavesEntity = new Entity(&m_scene); // Create the entity
	wavesEntity->SetName("Waves");
	wavesEntity->GetTransform()->SetScale(glm::vec3(10, 1, 10));
	MeshFilter* waves = new MeshFilter(wavesEntity, ANIMATINGSOLID); // Create its mesh filter
	waves->LoadModel("models/Sea/Sea.obj"); // Load the model

#pragma endregion

	return true;
}

void MyApplication::Update(float a_deltaTime)
{
	// clear all gizmos from last frame
	Gizmos::clear();

	std::vector<SpotLight*> spots = m_scene.FindAllComponentsOfType<SpotLight>();

	if (spots.size() > 0)
	{
		if (spots[0] != nullptr && m_scene.m_activeCamera != nullptr)
		{
			spots[0]->SetDirection(-m_scene.m_activeCamera->GetOwnerEntity()->GetTransform()->GetForward());
		}
	}

	// add a 20x20 grid on the XZ-plane
	for (int i = 0; i < 21; ++i)	
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10),
			i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0.2, 0.2, 0.2, 1));

		Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i),
			i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0.2, 0.2, 0.2, 1));
	}

	m_renderer.OnGUI();
	

#pragma region ImGui

#pragma region Menu_Bar
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Create"))
		{
			Entity* newEntity = new Entity(&m_scene);
		}

		if (ImGui::MenuItem("Delete"))
		{
			m_scene.Delete();
		}

		if (ImGui::MenuItem("Show Explorer", "", m_showUI))
		{
			m_showUI = !m_showUI;
		}

		if (ImGui::MenuItem("Show Renderer", "", m_renderer.showGUI))
		{
			m_renderer.showGUI = !m_renderer.showGUI;
		}

		if (ImGui::MenuItem("Quit"))
		{
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}


		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Add Component"))
		{
			bComponentTool = true;
			bComponentAdd = true;
		}
		if (ImGui::MenuItem("Delete Component"))
		{
			bComponentTool = true;
			bComponentAdd = false;
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

#pragma endregion Menu_Bar

#pragma region Component_Tool

	if (bComponentTool)
	{
		ImGui::Begin("Select Component", NULL, ImGuiWindowFlags_NoCollapse);
		ImGui::BeginChild("Scrolling");
		if (bComponentAdd)
		{
			// List components here

			if (ImGui::Button("Mesh Filter"))
			{
				//MeshFilter* newComp = new MeshFilter(m_scene.selectedEntity);
				//bComponentTool = false;
				CL_INFO("Not implemented.");
			}


			ImGui::NewLine();
			if (ImGui::Button("Cancel"))
			{
				bComponentTool = false;
			}
		}
		else
		{
			std::vector<Component*>* comps = m_scene.selectedEntity->GetComponentList();

			for (int i = 0; i < m_scene.selectedEntity->GetComponentList()->size(); i++)
			{
				if (comps->at(i)->OnDelete())
				{
					bComponentTool = false;
				}
			}
			
			ImGui::NewLine();
			if (ImGui::Button("Cancel"))
			{
				bComponentTool = false;
			}
		}

		ImGui::EndChild();
		ImGui::End();
	}


#pragma endregion Component_Tool

#pragma region Inspector

	
	if (m_showUI)
	{
		ImGui::Begin("World Outliner", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

		ImGui::TextColored(ImVec4(0, 170, 0, 1), "Entity List:");
		ImGui::BeginChild("Scrolling");

		if (m_scene.m_sceneEntities.capacity() > 0)
		{
			std::string id = "";
			for (int i = 0; i < m_scene.m_sceneEntities.size(); i++)
			{
				id = *m_scene.m_sceneEntities.at(i)->GetName();

				ImGui::Text("%s", id.c_str());
				ImGui::SameLine(ImGui::GetWindowWidth() - 80);

				ImGui::PushID(i);
				if (ImGui::Button("Select"))
				{
					m_scene.selectedEntity = m_scene.m_sceneEntities.at(i);
				}
				ImGui::PopID();
			}
			ImGui::EndChild();
		}
		else
		{
			ImGui::EndChild();
		}


		ImGui::End();


		ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
		ImGui::TextColored(ImVec4(170, 0, 0, 1), "Selected Entity:");

		if (m_scene.selectedEntity != nullptr)
		{
			m_scene.selectedEntity->OnGUI();
		}


		ImGui::End();

	}


#pragma endregion Inspector

#pragma endregion ImGui

	m_scene.Update(a_deltaTime);

	// quit our application when escape is pressed  TODO: replace with save and exit
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		quit();
}

void MyApplication::Draw()
{
	m_renderer.Draw(&m_scene);

	// Needs to be called last
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyApplication::Destroy()
{
	Gizmos::destroy();
}

