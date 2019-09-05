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
	isWireframe(false),
	specularAmount(32.0f),
	gammaCorrection(1.8f),
	screenshot(false)
{

}

Game::~Game()
{
}

bool Game::OnCreate()
{
	glEnable(GL_DEPTH_TEST);
	if (isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	fboWidth = 1;
	fboHeight = 1;

	// Creating the FBO
	glGenFramebuffers(1, &defaultFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);

	// Create a color attachment texture
	glGenTextures(1, &defaultColourTex);
	glBindTexture(GL_TEXTURE_2D, defaultColourTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fboWidth, fboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, defaultColourTex, 0);

	// Create a renderbuffer object for depth attachment (not a texture because we're not sampling)
	glGenRenderbuffers(1, &depthRender);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRender);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fboWidth, fboHeight); // Create the depth render buffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRender); // Attach the render buffer object to the FBO depth slot

	// Test that the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	cam = std::make_shared<Caramel::Camera>();

	// Create the user picked shape
	shape = InitShape();

	// Setting up the shaders
	simpleShader = Caramel::Shader::CreateShader("resources/shaders/simple.glsl");
	lightShader = Caramel::Shader::CreateShader("resources/shaders/light.glsl");
	shaderProgram = simpleShader->GetProgramID();
	lightProgram = lightShader->GetProgramID();

	// Setting up textures
	newTexture = Caramel::Texture::CreateTexture("resources/textures/test3.jpg");

	// Setting up the light 
	light = std::make_shared<Cube>();
	light->SetPosition(glm::vec3(0, 2, -5));
	light->Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	lightColour = glm::vec3(1, 0.5f, 1);

	simpleShader->Bind();
	simpleShader->SetInt("gMaterial.texture", 0);

	return true;
}

void Game::Update(float a_deltaTime)
{
	shape->Rotate(a_deltaTime, glm::vec3(0, 1, 0));
	cam->Update(a_deltaTime);
}

void Game::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/// DRAW
	if (simpleShader->Bind())
	{
		simpleShader->SetVec3("gLight.pos", light->GetPos());
		simpleShader->SetVec3("gLight.colour", lightColour);
		simpleShader->SetFloat("gMaterial.spec", specularAmount);
		simpleShader->SetFloat("gGamma", gammaCorrection);
		newTexture->Bind(GL_TEXTURE0);
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


	if (screenshot)
	{
		Screenshot();
	}

	ImGuiDraw();
}

void Game::Destroy()
{
}

void Game::ImGuiDraw()
{
	// Draw ImGui on the backbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	ImTextureID texID;

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
	ResizeFBO(viewportSize.x, viewportSize.y); // Resize the fbo to match the viewport size
	cam->SetProjectionMatrix(glm::perspective(glm::radians(50.0f), viewportSize.x / viewportSize.y, 0.1f, 5000.0f));
	texID = (void*)(intptr_t)defaultColourTex;
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
	texID = (void*)(intptr_t)newTexture->GetID();

	if (ImGui::ImageButton(texID, { 80, 80 }, ImVec2(0, 1), ImVec2(1, 0)))
	{
		newTexture->Reload(Caramel::Utility::OpenFileDialog(m_window->GetNative()));
	}
	ImGui::SameLine();
	ImGui::Text("Texture");
	ImGui::DragFloat("Specularity", &specularAmount, 0.05f);
	ImGui::End();

	ImGui::Begin("Rendering", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Properties");
	ImGui::Separator();
	if (ImGui::Button("Screenshot"))
	{
		screenshot = true;
	}
	static bool isFullscreen = Caramel::AppWindow::IsFullscreen();
	if (ImGui::Checkbox("Toggle fullscreen", &isFullscreen))
	{
		Caramel::AppWindow::SetFullscreen(isFullscreen);
	}
	if (ImGui::Checkbox("Wireframe", &isWireframe))
	{
		if (isWireframe)
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
	ImGui::DragFloat("Gamma", &gammaCorrection, 0.05f);
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

void Game::ResizeFBO(float a_width, float a_height)
{
	if (fboWidth == a_width && fboHeight == a_height)
	{
		return;
	}

	glDeleteFramebuffers(1, &defaultFBO);
	glDeleteTextures(1, &defaultColourTex);
	glDeleteRenderbuffers(1, &depthRender);

	fboWidth = a_width;
	fboHeight = a_height;

	// Creating the FBO
	glGenFramebuffers(1, &defaultFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);

	// Create a color attachment texture
	glGenTextures(1, &defaultColourTex);
	glBindTexture(GL_TEXTURE_2D, defaultColourTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fboWidth, fboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, defaultColourTex, 0);

	// Create a renderbuffer object for depth attachment (not a texture because we're not sampling)
	glGenRenderbuffers(1, &depthRender);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRender);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fboWidth, fboHeight); // Create the depth render buffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRender); // Attach the render buffer object to the FBO depth slot

	// Test that the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glViewport(0, 0, fboWidth, fboHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	viewPortSize.x = a_width;
	viewPortSize.y = a_height;
}

void Game::Screenshot()
{
	screenshot = false;

	auto viewportSize = ImGui::GetContentRegionAvail();

	int width = viewPortSize.x;
	int height = viewPortSize.y;

	int imageSize = width * height * 3;

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Create buffer for images
	char* dataBuffer = new char[imageSize];

	// Grab pixels
	glReadPixels((GLint)0, (GLint)0,
		(GLint)width, (GLint)height,
		GL_RGB, GL_UNSIGNED_BYTE, dataBuffer);


	// Save to file
	stbi_flip_vertically_on_write(true);
	stbi_write_png("image.png", width, height, 3, (void*)dataBuffer, width * 3);

	// Free resources
	delete[] dataBuffer;
}

std::shared_ptr<Shape> Game::InitShape()
{
	std::cout << "\n1. Pyramid\n2. Cube\n";

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
