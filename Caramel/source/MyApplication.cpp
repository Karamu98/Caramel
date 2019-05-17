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
#include "Defines.h"

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

	Entity* newTest = new Entity(&m_scene);
	newTest->SetName("Test");

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

	// Display the renderer's GUI
	m_renderer.OnGUI();
	

#pragma region ImGui

#pragma region Menu_Bar

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		//if (ImGui::MenuItem("Open Scene"))
		//{
		//	// File dialog
		//}

		//if (ImGui::MenuItem("Save Scene", "Ctrl + S"))
		//{
		//	Save("");
		//}

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

	if (ImGui::BeginMenu("Entity"))
	{
		if (ImGui::MenuItem("Create Empty", "Ctrl + N"))
		{
			Entity* newEntity = new Entity(&m_scene);
		}

		if (ImGui::MenuItem("Delete Selected", "Del"))
		{
			m_scene.Delete();
		}

		ImGui::EndMenu();
	}

	if (m_scene.selectedEntity != nullptr)
	{
		if (ImGui::BeginMenu("Component"))
		{
			if (ImGui::BeginMenu("Mesh"))
			{
				if (ImGui::MenuItem("Mesh Filter"))
				{
					MeshFilter* newMesh = new MeshFilter(m_scene.selectedEntity);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Rendering"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					Camera* newCam = new Camera(m_scene.selectedEntity);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Directional Light"))
				{
					DirectionalLight* newLight = new DirectionalLight(m_scene.selectedEntity);
				}

				if (ImGui::MenuItem("Point Light"))
				{
					PointLight* newLight = new PointLight(m_scene.selectedEntity);
				}

				if (ImGui::MenuItem("Spot Light"))
				{
					SpotLight* newLight = new SpotLight(m_scene.selectedEntity);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

	}

	ImGui::EndMainMenuBar();

#pragma endregion Menu_Bar

#pragma region Outliner & Inspector
	
	if (m_showUI)
	{
		//  World outliner
		if (ImGui::Begin("World Outliner", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			ImGui::TextColored(ImVec4(0, 170, 0, 1), "Entity List:");

			if (m_scene.m_sceneEntities.capacity() > 0)
			{
				ImGui::BeginChild("Scrolling");

				std::string id = "";
				for (int i = 0; i < m_scene.m_sceneEntities.size(); i++)
				{
					// Test if they're selected
					bool isSelected = false;
					if (m_scene.m_sceneEntities.at(i) == m_scene.selectedEntity)
					{
						isSelected = true;
					}

					// List them in the inspector and select them if they're selected
					id = m_scene.m_sceneEntities.at(i)->GetName();
					ImGui::PushID(i);
					if (ImGui::Selectable(id.c_str(), isSelected))
					{
						m_scene.selectedEntity = m_scene.m_sceneEntities.at(i);
					}
					ImGui::PopID();
				}
				ImGui::EndChild();

			}

			ImGui::End();
		}


		// Inspector
		if (ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			ImGui::TextColored(ImVec4(170, 0, 0, 1), "Selected Entity:");

			if (m_scene.selectedEntity != nullptr)
			{
				m_scene.selectedEntity->OnGUI();
			}

			//ImGui::NewLine();
			//ImGui::NewLine();
			//
			//if (ImGui::Button("Add Component"))
			//{
			//	CL_CORE_WARN("Not implemented.");
			//}

			ImGui::End();
		}
	}

#pragma endregion Outliner & Inspector

#pragma endregion ImGui

#pragma region EditorInput

	// Delete entities

	if(ImGui::IsKeyPressed(261))
	{
		m_scene.Delete();
	}

	// Creating entities shortcut
	if (ImGui::IsKeyDown(341) && ImGui::IsKeyPressed(78)) // Ctrl + N
	{
		Entity* newEntity = new Entity(&m_scene);
	}

	if (ImGui::IsKeyDown(341) && ImGui::IsKeyPressed(68)) // Ctrl + D
	{
		Entity* newEntity = new Entity(*m_scene.selectedEntity, &m_scene);
	}

	//if (ImGui::IsKeyDown(341) && ImGui::IsKeyPressed(83)) // Ctrl + S
	//{
	//	Save("");
	//}


#pragma endregion

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

// Saving and loading may be improved using boost::serilization
// https://theboostcpplibraries.com/boost.serialization-class-hierarchies
// =======================

void MyApplication::Save(std::string a_filePath)
{
	
	std::ofstream outFile("save.sweet", std::ios::binary);

	if (outFile)
	{
		m_renderer.Save(&outFile);

		m_scene.Save(&outFile);

		outFile.close();
		CL_CORE_INFO("Scene saved.");
	}
	else
	{
		CL_CORE_WARN("Unable to save.");
	}


}

void MyApplication::Load(std::string a_filePath)
{
	std::ifstream inFile("save.sweet", std::ios::binary);

	if (inFile)
	{
		m_renderer.Load(&inFile);

		m_scene.Load(&inFile);

		inFile.close();
		CL_CORE_INFO("Scene Loaded.");
	}
	else
	{
		CL_CORE_WARN("Unable to load scene: {0}.", a_filePath);
	}
}

