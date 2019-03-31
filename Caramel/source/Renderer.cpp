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
#include "Defines.h"
#include "PointLight.h"




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


	// Create the default deferred rendering shaders
	m_firstPass = new Shader("shaders/deferredVertex", "shaders/deferredFrag");

	// Create the second pass shader for this
	m_secondLight = new Shader("shaders/defSecondV", "shaders/defSecondFrag");

#pragma region Creating first pass buffer

	// Creating the buffer
	glGenFramebuffers(1, &m_firstPassBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_firstPassBuffer);

	// Setting up the position buffer (x, y, z for RGB)
	glGenTextures(1, &m_posBufferID);
	glBindTexture (GL_TEXTURE, m_posBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_posBufferID, 0);

	// Setting up the normal buffer (x, y, z for RGB)
	glGenTextures(1, &m_normBuffer);
	glBindTexture(GL_TEXTURE, m_normBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normBuffer, 0);

	// Setting up the colour buffer with the specular as addition (r, g, b is colour. a is spec)
	glGenTextures(1, &m_colandSpecID);
	glBindTexture(GL_TEXTURE, m_colandSpecID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_colandSpecID, 0);

	unsigned int attachments[3] = { m_posBufferID, m_normBuffer, m_colandSpecID };
	glDrawBuffers(3, attachments);

#pragma endregion

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

#pragma region Deferred

	/// First pass
	glBindFramebuffer(GL_FRAMEBUFFER, m_firstPassBuffer); // Bind the framebuffer for deferred
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear it

	m_firstPass->Bind(); // Bind the first pass shader and pass uniforms
	m_firstPass->SetMat4("projectionView", activeCam->GetProjectionView());
	m_firstPass->SetMat4("viewMatrix", activeCam->GetViewMatrix());
	m_firstPass->SetVec4("camPos", activeCam->GetCameraMatrix()[3]);
	
	std::vector<MeshFilter*> meshes = a_sceneToRender->FindAllComponentsOfType<MeshFilter>();

	for (MeshFilter* mesh : meshes)
	{
		mesh->Draw(m_firstPass);
	}

	/// Second light pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind the default frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear it

	// Bind the textures for the shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE, m_posBufferID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE, m_normBuffer);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE, m_colandSpecID);

	m_secondLight->Bind();

	m_secondLight->SetVec3("viewPos", activeCam->GetCameraMatrix()[3]);
	// Send all the lights with their appropiate data to the shader
	std::vector<Light*> lights = a_sceneToRender->FindAllComponentsOfType<Light>();
	for(Light* light : lights)
	{
		light->Draw(m_secondLight);
	}

	// RenderQuad???

	// Send depth to second pass



	





#pragma endregion

	// First pass deferred

	for (auto &shader : m_shaders )
	{
		shader.second->Bind();
		shader.second->SetFloat("Time", Utility::getTotalTime());
		shader.second->SetMat4("ProjectionView", activeCam->GetProjectionView());
		shader.second->SetMat4("ViewMatrix", activeCam->GetViewMatrix());
		shader.second->SetVec4("cameraPosition", activeCam->GetCameraMatrix()[3]);
		shader.second->SetVec4("lightDirection", glm::vec4(0, -0.5f, 1, 1));
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
