#include "Renderer.h"
#include "Component.h"
#include "Scene.h"
#include "Camera.h"
#include "Gizmos.h"
#include "Log.h"
#include "imgui.h"
#include "gl_core_4_4.h"
#include "MeshFilter.h"
#include "Scene.h"
#include "Entity.h"



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
		Component* comp;
		if (activeCamera == nullptr)
		{
			comp = entity->FindComponentOfType(COMPONENT_TYPE::CAMERA);
			if (comp != nullptr)
			{
				activeCamera = static_cast<Camera*>(comp);
			}
		}

		comp = entity->FindComponentOfType(COMPONENT_TYPE::MESHFILTER);
		
		if(comp == nullptr)
		{
			continue;
		}

		MeshFilter* meshFilter = static_cast<MeshFilter*>(comp);

		meshFilter->Draw(activeCamera);
	}

	// Draw the gizmos from this frame
	Gizmos::draw(glm::inverse(activeCamera->GetCameraMatrix()), activeCamera->GetProjectionMatrix());
}

void Renderer::OnGUI()
{
	
}
