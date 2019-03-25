#include "Renderer.h"
#include "Component.h"
#include "Scene.h"
#include "Gizmos.h"
#include "Log.h"
#include "imgui.h"
#include "gl_core_4_4.h"
#include "MeshFilter.h"
#include "Scene.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Camera.h"




Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Init(bool a_isDeferred)
{
	// Set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f, 0.25f, 0.25f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	CL_CORE_INFO("Renderer Initialised.");
}

void Renderer::Draw(Scene* a_sceneToRender)
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (Entity* entity : a_sceneToRender->m_sceneEntities)
	{
		if (activeCamera == nullptr)
		{
			Camera* cam = entity->GetComponentOfType<Camera>();
			if (cam != nullptr)
			{
				activeCamera = cam;
			}
		}


		MeshFilter* mesh = entity->GetComponentOfType<MeshFilter>();
		
		if(mesh == nullptr)
		{
			continue;
		}

		MeshFilter* meshFilter = mesh;

		meshFilter->Draw(activeCamera);
	}

	// Draw the gizmos from this frame
	Gizmos::draw(glm::inverse(activeCamera->GetCameraMatrix()), activeCamera->GetProjectionMatrix());
}

void Renderer::OnGUI()
{
	
}
