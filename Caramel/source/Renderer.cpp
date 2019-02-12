#include "Renderer.h"
#include "Component.h"
#include "Scene.h"
#include "Camera.h"
#include "GLFW/glfw3.h"
#include "Material.h"
#include "Gizmos.h"
#include "Log.h"



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Init(bool a_isDeferred)
{
	m_bDeferredRendering = a_isDeferred;

	/// Set up default material
}

void Renderer::Draw(Scene* a_sceneToRender)
{
	/// BIND

	if (m_bDeferredRendering)
	{
		DeferredRendering(a_sceneToRender);
	}
	else
	{
		ForwardRendering(a_sceneToRender);
	}

	/// UNBIND: This is redundant, not unbinding is unsafe but safes processing
	// Draw the gizmos from this frame
	Gizmos::draw(glm::inverse(activeCamera->GetCameraMatrix()), activeCamera->GetProjection());
}

/// <summary>
/// Does the default amount of rendering before spliting into a technique
/// </summary>
/// <param name="a_sceneToRender">The scene in which we are going to render.</param>
void Renderer::Render(Scene* a_sceneToRender)
{
	Component* comp = a_sceneToRender->FindComponentOfType(COMPONENT_TYPE::CAMERA);
	if (comp == nullptr)
	{
		return;
	}
	activeCamera = dynamic_cast<Camera*>(comp);
	glm::mat4 viewMatrix = glm::inverse(activeCamera->GetCameraMatrix());
	

	/*
	Grab active camera
	Grab all active meshes
	====================== Vertex Shading
	Grab all primitives
	====================== Geometry Shading
	====================== Tessalation	
	*/
}

void Renderer::ForwardRendering(Scene * a_sceneToRender)
{
	Render(a_sceneToRender);
	if (activeCamera == nullptr)
	{
		CL_CORE_WARN("No cameras rendering.");
		return;
	}
	/*
	Grab all materials of parsed meshes
	Grab all lights
	====================== Fragment Shading
	*/
}


void Renderer::DeferredRendering(Scene * a_sceneToRender)
{
	Render(a_sceneToRender);
	if (activeCamera == nullptr)
	{
		CL_CORE_WARN("No cameras rendering.");
		return;
	}
	/*
	Grab all materials of parsed meshes
	Grab all lights
	====================== Fragment Shading (1/2)
	.... TO CONTINUE
	*/
}

