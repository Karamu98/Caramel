#include "Renderer.h"
#include "Component.h"
#include "Scene.h"
#include "Camera.h"
#include "Material.h"
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
	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f, 0.25f, 0.25f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Compile shaders and link

	m_bDeferredRendering = a_isDeferred;

	CL_CORE_INFO("Renderer Initialised.");

	glGenBuffers(1, &m_vBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Define where position is in our buffer
	glEnableVertexAttribArray(0); // Enable this attribute

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

	for (int i = 0; i < a_sceneToRender->m_sceneEntities.size(); i++)
	{
		MeshFilter* mesh = (MeshFilter*)a_sceneToRender->m_sceneEntities[i]->FindComponentOfType(COMPONENT_TYPE::MESHFILTER);

		if (mesh != nullptr)
		{
			for (int v = 0; v < mesh->vertCount; v++)
			{
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &mesh->verts[v].position, GL_DYNAMIC_DRAW); // Pass the positions
				//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &mesh->verts[v].normals, GL_STATIC_DRAW); // Pass the normals
				//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &mesh->verts[v].tans, GL_DYNAMIC_DRAW); // Pass the Tans
				//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &mesh->verts[v].biTans, GL_DYNAMIC_DRAW); // Pass the BiTans
				//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2, &mesh->verts[v].uvs, GL_DYNAMIC_DRAW); // Pass the UVs
			}
		}
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

