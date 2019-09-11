#include "Game.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_opengl3.cpp"
#include "examples/imgui_impl_glfw.cpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <iostream>
#include <glm/ext.hpp>


Game::Game() :
	cubeBrightness(8.0f),
	cubeShine(32.0f),
	shouldScreenshot(false),
	m_gammaCorrection(1.8f),
	m_renderWireFrame(false)
{

}

Game::~Game()
{
}

bool Game::OnCreate()
{
	cam = std::make_shared<Caramel::Camera>();

	// Create the user picked shape
	shape = InitShape();

	m_viewFramebuffer = std::make_shared<Caramel::Framebuffer>();

	std::string workingDir = Caramel::Utility::GetWorkingDir();
	// Setting up the shaders
	simpleShader = Caramel::Shader::CreateShader(workingDir + "resources/shaders/simple.glsl");
	lightShader = Caramel::Shader::CreateShader(workingDir + "resources/shaders/light.glsl");
	shaderProgram = simpleShader->GetProgramID();
	lightProgram = lightShader->GetProgramID();

	// Setting up textures
	cubeDiffuse = Caramel::Texture::CreateTexture(workingDir + "resources/textures/container.png");
	cubeSpecular = Caramel::Texture::CreateTexture(workingDir + "resources/textures/container_specular.png");
	cubeEmission = Caramel::Texture::CreateTexture(workingDir + "resources/textures/container_emissive.png");

	// Setting up the skybox
	const std::vector<std::string> faces = {
		workingDir + "resources/textures/skybox/right.jpg",
		workingDir + "resources/textures/skybox/left.jpg",
		workingDir + "resources/textures/skybox/top.jpg",
		workingDir + "resources/textures/skybox/bottom.jpg",
		workingDir + "resources/textures/skybox/front.jpg",
		workingDir + "resources/textures/skybox/back.jpg"
	};
	Caramel::Skybox::SetSkybox(faces);

	// Setting up the light 
	light = std::make_shared<Cube>();
	light->SetPosition(glm::vec3(0, 2, -5));
	light->Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	lightColour = glm::vec3(1, 0.5f, 1);

	simpleShader->Bind();
	simpleShader->SetInt("gMaterial.texture", 0);
	simpleShader->SetInt("gMaterial.spec", 1);
	simpleShader->SetInt("gMaterial.emission", 2);

	return true;
}

void Game::Update(float a_deltaTime)
{
	shape->Rotate(a_deltaTime, glm::vec3(0, 1, 0));
	cam->Update(a_deltaTime);
}

void Game::ImDraw()
{
	// Draw ImGui on the backbuffer as we're drawing the scene as a texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	static bool p_open = true;

	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");
	auto viewportSize = ImGui::GetContentRegionAvail();
	portSize = { viewportSize.x, viewportSize.y };
	m_viewFramebuffer->ResizeFBO(viewportSize.x, viewportSize.y); // Resize the fbo to match the viewport size
	cam->SetProjectionMatrix(glm::perspective(glm::radians(50.0f), viewportSize.x / viewportSize.y, 0.1f, 5000.0f));
	ImTextureID texID = (void*)(intptr_t)m_viewFramebuffer->m_colourTex;
	ImGui::Image(texID, viewportSize, { 0, 1 }, { 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();


	ImGui::Begin("Light", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Properties");
	ImGui::Separator();
	ImGui::DragFloat3("Position", glm::value_ptr((*light->GetMatrix())[3]), 0.1f);
	ImGui::ColorEdit3("Colour", glm::value_ptr(lightColour));

	ImGui::End();

	ImGui::Begin("Material", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Properties");
	ImGui::Separator();

	Caramel::Utility::TextureButton("Diffuse", cubeDiffuse);
	Caramel::Utility::TextureButton("Specular", cubeSpecular);
	ImGui::SameLine();
	ImGui::DragFloat("Shine", &cubeShine, 0.05f, 0, 8096);
	Caramel::Utility::TextureButton("Emissive", cubeEmission);
	ImGui::SameLine();
	ImGui::DragFloat("Brightness", &cubeBrightness, 0.05f, 0.0f, 1000.0f);


	ImGui::End();

	ImGui::Begin("Rendering", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Properties");
	ImGui::Separator();
	static float timeBuf = 1;
	if (ImGui::DragFloat("Time Dilation", &timeBuf, 0.05f))
	{
		Caramel::Application::SetTimeDilation(timeBuf);
	}
	if (ImGui::Button("Screenshot"))
	{
		shouldScreenshot = true;
	}
	static bool isFullscreen = Caramel::AppWindow::IsFullscreen();
	if (ImGui::Checkbox("Toggle fullscreen", &isFullscreen))
	{
		Caramel::AppWindow::SetFullscreen(isFullscreen);
	}
	bool* wFrame = &m_renderWireFrame;
	if (ImGui::Checkbox("Wireframe", wFrame))
	{
		if (*wFrame)
		{
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
	}
	ImGui::DragFloat("Gamma", &m_gammaCorrection, 0.05f);
	ImGui::Separator();
	static char buf[32] = "";
	if (ImGui::InputText("Window title", buf, 32))
	{
		Caramel::AppWindow::SetTitle(buf);
	}
	ImGui::Text("Application Average: %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (ImGui::IsMousePosValid())
		ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
	else
		ImGui::Text("Mouse Position: <invalid>");

	ImGui::Text("Window size: %.ux%.u", Caramel::AppWindow::GetWidth(), Caramel::AppWindow::GetHeight());
	ImGui::Text("Window focus: %.i", (int)Caramel::AppWindow::IsFocused());

	ImGui::End();

	ImGui::Begin("Shader Reloading");
	if (ImGui::Button("Simple Shader"))
	{
		simpleShader->Recompile();
	}
	if (ImGui::Button("Light Shader"))
	{
		lightShader->Recompile();
	}
	ImGui::End();

	ImGui::End();
}

void Game::Draw()
{
	/// DRAW
	m_viewFramebuffer->Bind();

	Caramel::Skybox::Draw(*cam);

	if (simpleShader->Bind())
	{
		simpleShader->SetVec3("gLight.pos", light->GetPos());
		simpleShader->SetVec3("gLight.colour", lightColour);
		simpleShader->SetFloat("gMaterial.shininess", cubeShine);
		simpleShader->SetFloat("gMaterial.emissionBrightness", cubeBrightness);
		simpleShader->SetFloat("gGamma", m_gammaCorrection);
		cubeDiffuse->Bind(GL_TEXTURE0);
		cubeSpecular->Bind(GL_TEXTURE1);
		cubeEmission->Bind(GL_TEXTURE2);
		cam->Draw(shaderProgram);
		shape->Draw(shaderProgram);
	}

	if (lightShader->Bind())
	{
		lightShader->SetVec3("gLightColour", lightColour);
		cam->Draw(lightProgram);
		light->Draw(lightProgram);
		lightShader->Unbind();
	}

	if (shouldScreenshot)
	{
		shouldScreenshot = false;
		Caramel::Utility::Screenshot("image.png", portSize.x, portSize.y);
	}
}

void Game::Destroy()
{
}

std::shared_ptr<Shape> Game::InitShape()
{
	CL_TRACE("Pick a shape:\n1. Pyramid\n2. Cube\n");

	int option = 0;

	// Wait until we pick a valid option
	while (true)
	{
		std::cin >> option;
		switch (option)
		{
		case 1:
		{
			return std::make_shared<Pyramid>();
			break;
		}
		case 2:
		{
			return std::make_shared<Cube>();
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return nullptr;
}
