#include "Renderer.h"
#include "Component.h"
#include "Scene.h"
#include "Camera.h"
#include "GLFW/glfw3.h"
#include "Material.h"
#include "Gizmos.h"
#include "Log.h"
#include "imgui.h"



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Init(bool a_isDeferred)
{
	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f, 0.25f, 0.25f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Compile shaders and link

	m_bDeferredRendering = a_isDeferred;

	CL_CORE_INFO("Renderer Initialised.");

	/// Set up default material
}

void Renderer::Draw(Scene* a_sceneToRender)
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void Renderer::OnGUI()
{
	
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
	====================== Vertex Shading (Pass all vertex data in one clump)
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

