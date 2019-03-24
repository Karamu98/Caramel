#include "MyApplication.h"
#include "gl_core_4_4.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "Component.h"
#include "Log.h"
#include "MeshFilter.h"
#include "Camera.h"
#include "Entity.h"
#include "Renderer.h"



MyApplication::MyApplication()
{

}

MyApplication::~MyApplication()
{
	if (m_activeCam != nullptr)
	{
		delete m_activeCam;
	}
}

bool MyApplication::onCreate()
{
	// initialise the Gizmos helper class
	Gizmos::create();

	// Initialise ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_Init(m_window, true);
	ImGui::StyleColorsDark();

	// Initialise logging system
	Log::Init();
	CL_CORE_INFO("Initialised.");

	// Initialise Renderer
	m_renderer.Init(false);

	// Add the default entity.
	Entity* newEditor = new Entity();
	Camera* newCam = new Camera(newEditor);
	newEditor->AddComponent(newCam);
	m_scene.Add(newEditor);


	// Add the sword
	Entity* swordEntity = new Entity();
	m_modelShader = new Shader("shaders/modelVertex.glsl", "shaders/modelFragment.glsl");

	m_modelShader->Bind();


	MeshFilter* newMesh = new MeshFilter(swordEntity);
	swordEntity->AddComponent(newMesh);
	m_scene.Add(swordEntity);
	newMesh->SetShader(m_modelShader);
	newMesh->LoadModel("models/soulspear.fbx");
	
	

	return true;
}

void MyApplication::Update(float a_deltaTime)
{
	// Needs to be called first
	ImGui_ImplGlfwGL3_NewFrame();	

	// clear all gizmos from last frame
	Gizmos::clear();

	// add a 20x20 grid on the XZ-plane
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10),
			i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 1));

		Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i),
			i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 1));
	}

	


#pragma region ImGui

#pragma region Menu_Bar
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Create", "Ctrl+N"))
		{
			Entity* newEntity = new Entity();
			m_scene.Add(newEntity);
		}

		if (ImGui::MenuItem("Duplicate", "Ctrl+D"))
		{
			m_scene.Duplicate();
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
				m_scene.selectedEntity->AddComponent(newComp);
			}


			ImGui::NewLine();
			if (ImGui::Button("Cancel"))
			{
				bComponentTool = false;
			}
		}
		else
		{
			std::vector<Component*>::iterator xIter = m_scene.selectedEntity->GetComponentList()->begin();
			std::vector<Component*>::iterator end = m_scene.selectedEntity->GetComponentList()->end();

			for (xIter; xIter != end; xIter++)
			{
				Component* comp = *xIter;

				if (!comp)
				{
					return;
				}


				ImGui::PushID(xIter - m_scene.selectedEntity->GetComponentList()->begin());
				switch (comp->GetComponentType())
				{
				case TRANSFORM:
				{
					break;
				}
				default:
				{
					ptrdiff_t old = std::find(m_scene.selectedEntity->GetComponentList()->begin(), m_scene.selectedEntity->GetComponentList()->end(), comp) - m_scene.selectedEntity->GetComponentList()->begin();

					m_scene.selectedEntity->GetComponentList()->erase(m_scene.selectedEntity->GetComponentList()->begin() + old);
					bComponentTool = false;
					break;
				}
				}
				ImGui::PopID();
				if (!bComponentTool)
				{
					break;
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
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyApplication::Destroy()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	Gizmos::destroy();
}

