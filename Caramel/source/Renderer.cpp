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
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "glad/glad.h"
#include <string>


Renderer::Renderer():
m_renderWireframe(false)
{
}


Renderer::~Renderer()
{
	// Disable our rendering mode
	switch (m_currentMode)
	{
	case RenderingMode::FORWARD:
	{
		DisableForward();
		break;
	}
	case RenderingMode::DEFERRED:
	{
		DisableDeferred();
		break;
	}
	case RenderingMode::FORWARDPLUS:
	{
		DisableForwardPlus();
		break;
	}
	default:
		break;
	}

	// Remove shaders if present

}

void Renderer::Init(RenderingMode a_renderMode)
{
	// Set the clear colour and enable depth testing and backface culling
	glClearColor(0.0, 0.0, 0.0, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Set up out final pass buffer
	glGenFramebuffers(1, &m_finalFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_finalFramebuffer);

	// Our final colour texture
	glGenTextures(1, &m_finalColour);
	glBindTexture(GL_TEXTURE_2D, m_finalColour);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_finalColour, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	CL_GETGL_ERRORS;

	// Depth render buffer
	glGenRenderbuffers(1, &m_finalDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_finalDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_finalDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		CL_CORE_ERROR("Framebuffer not complete.");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	// Set up the rendering mode that has been picked
	switch (a_renderMode)
	{
	case RenderingMode::FORWARD:
	{
		InitForwardRendering();
		break;
	}
	case RenderingMode::DEFERRED:
	{
		InitDeferredRendering();
		break;
	}
	case RenderingMode::FORWARDPLUS:
	{
		InitForwardPlusRendering();
		break;
	}
	default:
		break;
	}
	m_currentMode = a_renderMode;

	CL_CORE_INFO("Renderer Initialised.");
}

void Renderer::Draw(Scene* a_sceneToRender)
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_renderWireframe) // TODO:: Allow for wireframe to be displayed on main window
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	Camera* activeCam = a_sceneToRender->GetActiveCamera();

	if (activeCam == nullptr)
	{
		return;
	}

	// Use the current mode to render to our target
	switch (m_currentMode)
	{
	case RenderingMode::FORWARD:
	{
		ForwardPass(a_sceneToRender, activeCam);
		break;
	}
	case RenderingMode::DEFERRED:
	{
		DeferredPass(a_sceneToRender, activeCam);
		break;
	}
	case RenderingMode::FORWARDPLUS:
	{
		ForwardPlusPass(a_sceneToRender, activeCam);
		break;
	}
	default:
		break;
	}


	// Pass over target with post processing
	

	// Draw on screen

	// Send depth to second pass, this is for blendables (transparents, unlit)
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_finalFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glUseProgram(0);

	// Draw the gizmos from this frame
	Gizmos::draw(glm::inverse(activeCam->GetCameraMatrix()), activeCam->GetProjectionMatrix());
}

void Renderer::OnGUI()
{
	ImGui::SetNextWindowPos(ImVec2(0, DEFAULT_SCREENHEIGHT - (DEFAULT_SCREENHEIGHT * 0.4f)));
	ImGui::SetNextWindowSize(ImVec2(DEFAULT_SCREENWIDTH * 0.3f, DEFAULT_SCREENHEIGHT * 0.4f));
	ImGui::Begin("Framebuffer");
	ImGui::Checkbox("Render wireframe", &m_renderWireframe);
	ImGui::BeginTabBar("Framebuffer textures");

	if (ImGui::BeginTabItem("Final Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_defGeoBuffer;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}


	if (ImGui::BeginTabItem("Position Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_posBuffer;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}


	if (ImGui::BeginTabItem("Colour Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_albedoBuffer;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Normal Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)m_normBuffer;
		ImGui::Image(texID, ImVec2(DEFAULT_SCREENWIDTH * 0.25f, DEFAULT_SCREENHEIGHT * 0.25f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::End();
}

void Renderer::ChangeRenderMode(RenderingMode a_newMode)
{
	// Saftey check
	if (a_newMode == RenderingMode::FORWARD || a_newMode == RenderingMode::FORWARDPLUS)
	{
		CL_CORE_ERROR("Not implemented!");
		return;
	}

	// Disable our old mode
	switch (m_currentMode)
	{
	case RenderingMode::FORWARD:
	{
		DisableForward();
		break;
	}
	case RenderingMode::DEFERRED:
	{
		DisableDeferred();
		break;
	}
	case RenderingMode::FORWARDPLUS:
	{
		DisableForwardPlus();
		break;
	}
	default:
		break;
	}

	// Enable our new one
	switch (a_newMode)
	{
	case RenderingMode::FORWARD:
	{
		InitForwardRendering();
		break;
	}
	case RenderingMode::DEFERRED:
	{
		InitDeferredRendering();
		break;
	}
	case RenderingMode::FORWARDPLUS:
	{
		InitForwardPlusRendering();
		break;
	}
	default:
		break;
	}

	m_currentMode = a_newMode;
}

void Renderer::InitForwardRendering()
{

}

void Renderer::InitDeferredRendering()
{
	// Create the solids shader
	m_defGeo = new Shader("shaders/deferredVertex.glsl", "shaders/deferredFrag.glsl", nullptr, "shaders/tessCTRL.glsl", "shaders/tessEVAL.glsl");

	// Create the transparents shader
	m_defGeoTransp = new Shader("shaders/defTranspVertex.glsl", "shaders/defTranspFrag.glsl", nullptr, "shaders/tessCTRL.glsl", "shaders/tessEVAL.glsl");

	// Create the animated solids shader
	m_defGeoAnim = new Shader("shaders/defAnimVertex.glsl", "shaders/deferredFrag.glsl", nullptr, "shaders/tessCTRL.glsl", "shaders/tessEVAL.glsl");

	// Create the animated transparents shader
	m_defGeoTranspAnim = new Shader("shaders/defAnimTranspVertex.glsl", "shaders/defAnimTranspFrag.glsl", nullptr, "shaders/tessCTRL.glsl", "shaders/tessEVAL.glsl");

	// Create the second pass shader for lighting
	m_defLight = new Shader("shaders/defSecondV.glsl", "shaders/defSecondFrag.glsl");

	// Creating the buffer
	glGenFramebuffers(1, &m_defGeoBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_defGeoBuffer);

	// Setting up the position buffer (x, y, z for RGB)
	glGenTextures(1, &m_posBuffer);
	glBindTexture(GL_TEXTURE_2D, m_posBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_posBuffer, 0);

	// Setting up the normal buffer (x, y, z for RGB)
	glGenTextures(1, &m_normBuffer);
	glBindTexture(GL_TEXTURE_2D, m_normBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normBuffer, 0);

	// Setting up the colour buffer
	glGenTextures(1, &m_albedoBuffer);
	glBindTexture(GL_TEXTURE_2D, m_albedoBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_albedoBuffer, 0);
	
	// Setting up the specular buffer
	glGenTextures(1, &m_specBuffer);
	glBindTexture(GL_TEXTURE_2D, m_specBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_specBuffer, 0);
	
	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	// Renderbuffer for depth to pass to default buffer
	glGenRenderbuffers(1, &m_rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		CL_CORE_ERROR("Framebuffer not complete.");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_defLight->Bind();
	m_defLight->SetInt("gPosition", 0);
	m_defLight->SetInt("gNormal", 1);
	m_defLight->SetInt("gAlbedo", 2);
	m_defLight->SetInt("gSpec", 3);

	m_defQuad = new Plane();	

	CL_GETGL_ERRORS;
}

void Renderer::InitForwardPlusRendering()
{
}

void Renderer::DisableForward()
{
}

void Renderer::DisableDeferred()
{
	delete m_defGeo;
	delete m_defLight;
	delete m_defQuad;

	glDeleteTextures(1, &m_posBuffer);
	glDeleteTextures(1, &m_normBuffer);
	glDeleteTextures(1, &m_albedoBuffer);
	glDeleteTextures(1, &m_specBuffer);
	glDeleteRenderbuffers(1, &m_rboDepth);
	glDeleteFramebuffers(1, &m_defGeoBuffer);
}

void Renderer::DisableForwardPlus()
{
}

void Renderer::ForwardPass(Scene * a_scene, Camera * a_activeCam)
{
}

void Renderer::DeferredPass(Scene* a_scene, Camera* a_activeCam)
{
	/// First pass
	glBindFramebuffer(GL_FRAMEBUFFER, m_defGeoBuffer); // Bind the framebuffer for deferred
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear it

	m_defGeo->Bind(); // Bind the first pass shader and pass uniforms
	m_defGeo->SetMat4("projectionView", a_activeCam->GetProjectionView());
	m_defGeo->SetMat4("viewMatrix", a_activeCam->GetViewMatrix());

	std::vector<MeshFilter*> meshes = a_scene->FindAllComponentsOfType<MeshFilter>();

	std::vector<MeshFilter*>::iterator xIter = meshes.begin();

#pragma region Solids
	/// Solid pass
	for (MeshFilter* mesh : meshes)
	{
		if (mesh->GetType() == MeshType::SOLID)
		{
			mesh->Draw(m_defGeo);
			meshes.erase(xIter);
		}
		if (meshes.size() > 1)
		{
			xIter++;
		}
	}

	/// Simple animating solid pass
	m_defGeoAnim->Bind();
	Utility::tickTimer();
	float time = Utility::getTotalTime();
	m_defGeoAnim->SetMat4("projectionView", a_activeCam->GetProjectionView());
	m_defGeoAnim->SetMat4("viewMatrix", a_activeCam->GetViewMatrix());
	m_defGeoAnim->SetFloat("Time", time);
	xIter = meshes.begin();
	for (MeshFilter* mesh : meshes)
	{
		if (mesh->GetType() == MeshType::ANIMATINGSOLID)
		{
			mesh->Draw(m_defGeoAnim);
			meshes.erase(xIter);
		}
		if (meshes.size() > 1)
		{
			xIter++;
		}
	}

#pragma endregion

#pragma region Transparents

	/// Transparent pass
	// Grab and sort all transparents by distance
	std::map<float, MeshFilter*> sortedTranspMeshes;
	for (MeshFilter* mesh : meshes)
	{
		if (mesh->GetType() == MeshType::TRANSPARENTS)
		{
			float distance = glm::length(glm::vec3(a_activeCam->GetCameraMatrix()[3]) - mesh->GetOwnerEntity()->GetTransform()->GetPosition());
			sortedTranspMeshes[distance] = mesh;
			meshes.erase(xIter);
		}
		if (meshes.size() > 1)
		{
			xIter++;
		}
	}

	// Draw them
	for (std::map<float, MeshFilter*>::reverse_iterator xIter = sortedTranspMeshes.rbegin(); xIter != sortedTranspMeshes.rend(); xIter++)
	{
		xIter->second->Draw(m_defGeoTransp);
	}

	/// Transparent animating pass
	// Grab and sort all transparents animating meshes into "sorteedSolidAnimMeshes"
	std::map<float, MeshFilter*> sortedTranspAnimMeshes;
	for (MeshFilter* mesh : meshes)
	{
		if (mesh->GetType() == MeshType::ANIMATINGTRANSPARENT)
		{
			float distance = glm::length(glm::vec3(a_activeCam->GetCameraMatrix()[3]) - mesh->GetOwnerEntity()->GetTransform()->GetPosition());
			sortedTranspAnimMeshes[distance] = mesh;
			meshes.erase(xIter);
		}
		if (meshes.size() > 1)
		{
			xIter++;
		}
	}

	// Draw them
	for (std::map<float, MeshFilter*>::reverse_iterator xIter = sortedTranspAnimMeshes.rbegin(); xIter != sortedTranspAnimMeshes.rend(); xIter++)
	{
		xIter->second->Draw(m_defGeoTranspAnim);
	}

#pragma endregion


	/// Second light pass
	glBindFramebuffer(GL_FRAMEBUFFER, m_finalFramebuffer); // Bind the output frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear it

	m_defLight->Bind();

	// Bind the textures for the shader that we populated by the first pass
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_posBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normBuffer);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_albedoBuffer);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, m_specBuffer);


	// For each of each type of light, pass them to the shader
	std::vector<DirectionalLight*> dirLights = a_scene->FindAllComponentsOfType<DirectionalLight>();
	std::vector<PointLight*> pointLights = a_scene->FindAllComponentsOfType<PointLight>();
	std::vector<SpotLight*> spotLights = a_scene->FindAllComponentsOfType<SpotLight>();

	m_defLight->SetInt("dirLightCount", dirLights.size());
	for (int i = 0; i < dirLights.size(); i++)
	{
		dirLights[i]->Draw(m_defLight, i);
	}

	m_defLight->SetInt("pointLightCount", pointLights.size());
	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i]->Draw(m_defLight, i);
	}

	m_defLight->SetInt("spotLightCount", spotLights.size());
	for (int i = 0; i < spotLights.size(); i++)
	{
		spotLights[i]->Draw(m_defLight, i);
	}
	m_defLight->SetVec3("viewPos", a_activeCam->GetCameraMatrix()[3]);

	m_defQuad->RenderPlane();

	// Send depth to final pass, this is for blendables (transparents, unlit)
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_defGeoBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_finalFramebuffer);
	glBlitFramebuffer(0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::ForwardPlusPass(Scene * a_scene, Camera * a_activeCam)
{
}
