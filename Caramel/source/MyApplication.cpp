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


MyApplication::MyApplication()
{

}

MyApplication::~MyApplication()
{
}

bool MyApplication::onCreate()
{
	// initialise the Gizmos helper class
	Gizmos::create();

	// Initialise ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui_ImplGlfw_InitForOpenGL(m_window, false);
	ImGui::StyleColorsDark();

	// Initialise logging system
	Log::Init();
	CL_CORE_INFO("Initialised.");

	// Initialise Renderer
	m_renderer.Init(RenderingMode::DEFERRED);

#pragma region Adding scene entities

	// Add the default entity.
	Entity* newEditor = new Entity(&m_scene);
	newEditor->SetName("Editor");
	Camera* newCam = new Camera(newEditor);
	newEditor->GetTransform()->SetPosition(glm::vec3(0, -3, 0));

	// Add a directional light and point light
	Entity* lightHolder = new Entity(&m_scene);
	lightHolder->SetName("Directional");
	DirectionalLight* sceneLight = new DirectionalLight(lightHolder);
	Entity* pointHolder = new Entity(&m_scene);
	pointHolder->SetName("Point");
	PointLight* pointLight = new PointLight(pointHolder);
	pointHolder->GetTransform()->SetPosition(glm::vec3(2, 2, 2));

	// Add the ruins
	Entity* ruinsEntity = new Entity(&m_scene); // Create the entity
	ruinsEntity->SetName("Ruins");
	ruinsEntity->GetTransform()->SetScale(glm::vec3(1, 1, 1));
	ruinsEntity->GetTransform()->SetPosition(glm::vec3(0, 1, 0));
	MeshFilter* ruins = new MeshFilter(ruinsEntity); // Create its mesh filter
	ruins->LoadModel("models/Jamie/soulspear.fbx"); // Load the model

	// Add the waves
	Entity* wavesEntity = new Entity(&m_scene); // Create the entity
	wavesEntity->SetName("Waves");
	wavesEntity->GetTransform()->SetScale(glm::vec3(3, 1, 3));
	MeshFilter* waves = new MeshFilter(wavesEntity); // Create its mesh filter
	//waves->LoadModel("models/Sea/Sea.obj"); // Load the model

#pragma endregion

	return true;
}

void MyApplication::Update(float a_deltaTime)
{
	// Needs to be called first
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	// clear all gizmos from last frame
	Gizmos::clear();

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
		if (ImGui::MenuItem("Create", "Ctrl+N"))
		{
			Entity* newEntity = new Entity(&m_scene);
			m_scene.Add(newEntity);
		}

		if (ImGui::MenuItem("Delete"))
		{
			m_scene.Delete();
		}


		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Add Component", "Ctrl+C"))
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

	if (ImGui::BeginMenu("Settings"))
	{
		if (ImGui::MenuItem("Renderer"))
		{
			ImGui::OpenPopup("Renderer Settings");
			CL_CORE_INFO("Not implemented.");
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
				MeshFilter* newComp = new MeshFilter(m_scene.selectedEntity);
				bComponentTool = false;
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

	ImGui::Begin("World Outliner", NULL, ImGuiWindowFlags_NoCollapse);

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
	

	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse);
	ImGui::TextColored(ImVec4(170, 0, 0, 1), "Selected Entity:");

	if (m_scene.selectedEntity != nullptr)
	{
		m_scene.selectedEntity->OnGUI();
	}
	

	ImGui::End();

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

