#include "Renderer.h"
#include "Component.h"
#include "Scene.h"
#include "Gizmos.h"
#include "Log.h"
#include "imgui.h"
#include "MeshFilter.h"
#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "Utilities.h"
#include "Defines.h"
#include "PointLight.h"
#include "glad/glad.h"




Renderer::Renderer()
{
}


Renderer::~Renderer()
{
	for (auto &shader : m_shaders)
	{
		delete shader.second;
	}

	delete m_defGeo;
	delete m_defLight;
}

void Renderer::Init(bool a_isDeferred)
{
	// Set the clear colour and enable depth testing and backface culling
	glClearColor(0.0, 0.0, 0.0, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// Create the default deferred rendering shaders
	m_defGeo = new Shader("shaders/deferredVertex.glsl", "shaders/deferredFrag.glsl");

	// Create the second pass shader for this
	m_defLight = new Shader("shaders/defSecondV.glsl", "shaders/defSecondFrag.glsl");

#pragma region Creating first pass buffer

	// Creating the buffer
	glGenFramebuffers(1, &m_defGeoBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_defGeoBuffer);

	// Setting up the position buffer (x, y, z for RGB)
	glGenTextures(1, &m_posBufferID);
	glBindTexture(GL_TEXTURE_2D, m_posBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_posBufferID, 0);

	// Setting up the normal buffer (x, y, z for RGB)
	glGenTextures(1, &m_normBuffer);
	glBindTexture(GL_TEXTURE_2D, m_normBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normBuffer, 0);

	// Setting up the colour buffer with the specular as addition (r, g, b is colour. a is spec)
	glGenTextures(1, &m_colandSpecID);
	glBindTexture(GL_TEXTURE_2D, m_colandSpecID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_colandSpecID, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenTextures(1, &m_depthBuffer);
	glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthBuffer, 0);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		CL_CORE_ERROR("Framebuffer not complete.");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_defLight->Bind();
	m_defLight->SetInt("gPosition", 0);
	m_defLight->SetInt("gNormal", 1);
	m_defLight->SetInt("gAlbedoSpec", 2);

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
	glBindFramebuffer(GL_FRAMEBUFFER, m_defGeoBuffer); // Bind the framebuffer for deferred
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear it

	m_defGeo->Bind(); // Bind the first pass shader and pass uniforms
	m_defGeo->SetMat4("projectionView", activeCam->GetProjectionView());
	m_defGeo->SetMat4("viewMatrix", activeCam->GetViewMatrix());
	
	std::vector<MeshFilter*> meshes = a_sceneToRender->FindAllComponentsOfType<MeshFilter>();

	for (MeshFilter* mesh : meshes)
	{
		mesh->Draw(m_defGeo);
	}

	/// Second light pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind the default frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear it

	m_defLight->Bind();

	// Bind the textures for the shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_posBufferID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normBuffer);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_colandSpecID);

	// Send all the lights with their appropiate data to the shader
	std::vector<Light*> lights = a_sceneToRender->FindAllComponentsOfType<Light>();
	for(int i = 0; i < lights.size(); i++)
	{
		lights[i]->Draw(m_defLight, i);
	}
	m_defLight->SetVec3("viewPos", activeCam->GetCameraMatrix()[3]);

	m_defQuad.RenderPlane();

	// Send depth to second pass, this is for blendables
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_defGeoBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Render blendables here




	





#pragma endregion

	// First pass deferred

	glUseProgram(0);

	// Draw the gizmos from this frame
	Gizmos::draw(glm::inverse(activeCam->GetCameraMatrix()), activeCam->GetProjectionMatrix());
}

void Renderer::OnGUI()
{
	ImGui::SetNextWindowPos(ImVec2(0, DEFAULT_SCREENHEIGHT - (DEFAULT_SCREENHEIGHT * 0.4f)));
	ImGui::SetNextWindowSize(ImVec2(DEFAULT_SCREENWIDTH * 0.3f, DEFAULT_SCREENHEIGHT * 0.4f));
	ImGui::Begin("Framebuffer");
	ImGui::BeginTabBar("Framebuffer textures");

	if (ImGui::BeginTabItem("Final Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_defGeoBuffer;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}


	if (ImGui::BeginTabItem("Position Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_posBufferID;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}


	if (ImGui::BeginTabItem("Colour Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_colandSpecID;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Normal Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_normBuffer;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}


	if (ImGui::BeginTabItem("Depth Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_depthBuffer;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::End();
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
