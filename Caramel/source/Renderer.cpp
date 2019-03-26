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
#include "Camera.h"
#include "Utilities.h"




Renderer::Renderer()
{
}


Renderer::~Renderer()
{
	for (auto &shader : m_shaders)
	{
		delete shader.second;
	}
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

	Camera* activeCam = a_sceneToRender->GetActiveCamera();

	if (activeCam == nullptr)
	{
		return;
	}

	for (auto &shader : m_shaders )
	{
		shader.second->Bind();
		shader.second->SetFloat("Time", Utility::getTotalTime());
		shader.second->SetMat4("ProjectionView", activeCam->GetProjectionView());
		shader.second->SetMat4("ViewMatrix", activeCam->GetViewMatrix());
		shader.second->SetVec4("cameraPosition", activeCam->GetCameraMatrix()[3]);
		shader.second->Draw();
	}
	glUseProgram(0);

	// Draw the gizmos from this frame
	Gizmos::draw(glm::inverse(activeCam->GetCameraMatrix()), activeCam->GetProjectionMatrix());
}

void Renderer::OnGUI()
{
	
}

Shader* Renderer::AddShader(const char* a_name, Shader* a_newShader)
{
	m_shaders[a_name] = a_newShader;

	return m_shaders[a_name];
}

Shader* Renderer::CreateShader(const char* a_name, const char* a_vertexPath, const char* a_fragPath, const char* a_geometryShader, const char* a_tessalationShader)
{
	m_shaders[a_name] = new Shader(a_vertexPath, a_fragPath, a_geometryShader, a_tessalationShader);

	return m_shaders[a_name];
}

Shader* Renderer::GetShader(const char* a_name)
{
	return m_shaders[a_name];
}
