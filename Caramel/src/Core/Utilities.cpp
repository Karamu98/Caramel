#include "clpch.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stdio.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <imgui.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Utilities.h"

#include "Core/Log.h"
#include "Render/Skybox.h"
#include "Render/Window.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace Caramel
{
	static double s_prevTime = 0;
	static float s_totalTime = 0;
	static float s_deltaTime = 0;

	std::string Utility::sm_workingDir;
	std::string Utility::sm_engineDir;

	void Utility::ResetTimer()
	{
		s_prevTime = glfwGetTime();
		s_totalTime = 0;
		s_deltaTime = 0;
	}

	float Utility::TickTimer()
	{
		double currentTime = glfwGetTime();
		s_deltaTime = (float)(currentTime - s_prevTime);
		s_totalTime += s_deltaTime;
		s_prevTime = currentTime;
		return s_deltaTime;
	}

	float Utility::GetDeltaTime()
	{
		return s_deltaTime;
	}

	float Utility::GetTotalTime()
	{
		return s_totalTime;
	}

	std::string Utility::OpenFileDialog(GLFWwindow* a_window, const std::string& a_filter)
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window(a_window);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "All\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	std::shared_ptr<std::string> Utility::ReadFile(const std::string& a_file)
	{
		// Read in the file
		std::shared_ptr<std::string> result = std::make_shared<std::string>();
		std::ifstream in;
		in.open(a_file, std::ios::in | std::ios::binary);
		if (in.is_open())
		{
			// Grab the size of the file
			in.seekg(0, std::ios::end);
			result->resize(in.tellg());

			// Seek to the beginning and read it all
			in.seekg(0, std::ios::beg);
			in.read(&(*result)[0], result->size());
			in.close();
			return result;
		}
		else
		{
			CL_CORE_ERROR("Couldn't open file {0}", a_file);
			return nullptr;
		}
	}

	std::string Utility::GetWorkingDir()
	{
		if (sm_workingDir != "")
		{
			return sm_workingDir;
		}
		char path[MAX_PATH] = "";
		GetModuleFileNameA(NULL, path, MAX_PATH);

		std::string dir(path);
		dir = dir.substr(0, dir.find_last_of("\\/"));
		sm_workingDir = dir + '\\';
		return sm_workingDir;
	}

	std::string Utility::GetEngineDir()
	{
		if (sm_engineDir != "")
		{
			return sm_engineDir;
		}
		char path[MAX_PATH] = "";
		GetModuleFileNameA(NULL, path, MAX_PATH);

		std::string dir(path);
		dir = dir.substr(0, dir.find_last_of("\\/"));
		sm_engineDir = dir + "\\..\\Caramel\\";
		return sm_engineDir;
	}

	void Utility::GetGLErrors()
	{
		while (true)
		{
			const GLenum err = glGetError();
			if (GL_NO_ERROR == err)
				break;

			CL_CORE_ERROR("GL Error: {0}", GetGLErrorStr(err));
		}
	}

	bool Utility::TextureButton(const std::string& a_textureName, const std::shared_ptr<Caramel::Texture>& a_texture)
	{
		ImTextureID texID = (void*)(intptr_t)a_texture->GetID();

		if (ImGui::ImageButton(texID, { 80, 80 }, ImVec2(0, 1), ImVec2(1, 0)))
		{
			a_texture->Reload(Caramel::Utility::OpenFileDialog(AppWindow::GetNative()));
			return true;
		}
		ImGui::SameLine();
		ImGui::Text(a_textureName.c_str());
		return false;
	}

	bool Utility::ModelButton(const std::string& a_textureName, const std::shared_ptr<Caramel::Model>& a_model)
	{
		ImTextureID texID = (void*)(intptr_t)a_model->GetPreviewTex();

		if (ImGui::ImageButton(texID, { 80, 80 }, ImVec2(0, 1), ImVec2(1, 0)))
		{
			a_model->Reload(Caramel::Utility::OpenFileDialog(AppWindow::GetNative()));
			return true;
		}
		ImGui::SameLine();
		ImGui::Text(a_textureName.c_str());
		return false;
	}

	bool Utility::SkyboxButton()
	{
		static bool skyboxEdit = false;

		if (ImGui::Button("Edit Skybox"))
		{
			skyboxEdit = true;
		}

		if (!skyboxEdit)
			return false;
		if (ImGui::Begin("Edit Skybox", &skyboxEdit))
		{
			// Show buttons for textures
			std::string workingDir = Caramel::Utility::GetWorkingDir();

			static char rightBuff[128] = {};
			static char leftBuff[128] = {};
			static char topBuff[128] = {};
			static char bottomBuff[128] = {};
			static char fBuff[128] = {};
			static char bBuff[128] = {};

			PathEntry("Right", &rightBuff[0], 128);
			PathEntry("Left", &leftBuff[0], 128);
			PathEntry("Top", &topBuff[0], 128);
			PathEntry("Bottom", &bottomBuff[0], 128);
			PathEntry("Front", &fBuff[0], 128);
			PathEntry("Back", &bBuff[0], 128);

			if (ImGui::Button("Apply"))
			{
				const std::vector<std::string> newBox =
				{
					rightBuff,
					leftBuff,
					topBuff,
					bottomBuff,
					fBuff,
					bBuff
				};

				Skybox::SetSkybox(newBox);
				return true;
			}
			ImGui::End();
		}
		return false;
	}

	bool Utility::PathEntry(const std::string& a_fieldName, char* a_buffer, size_t a_length)
	{
		ImGui::PushID(a_fieldName.c_str());
		ImGui::Text(a_fieldName.c_str());
		ImGui::SameLine();
		ImGui::InputText("", a_buffer, a_length);
		ImGui::SameLine();
		if (ImGui::Button("Browse"))
		{
			std::string newPath = OpenFileDialog(Caramel::AppWindow::GetNative());
			if (newPath.length() < a_length)
			{
				memcpy(a_buffer, newPath.c_str(), newPath.length());
				return true;
			}
			else
			{
				CL_CORE_ERROR("Path entry buffer too small");
			}
		}
		ImGui::PopID();
		return false;
	}

	void Utility::Screenshot(const std::string& a_imageName, unsigned int a_width, unsigned int a_height)
	{	
		std::string outPath = GetWorkingDir() + a_imageName;
		int imageSize = a_width * a_height * 3;

		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		// Create buffer for images
		char* dataBuffer = new char[imageSize];

		// Grab pixels
		glReadPixels((GLint)0, (GLint)0,
			(GLint)a_width, (GLint)a_height,
			GL_RGB, GL_UNSIGNED_BYTE, dataBuffer);


		// Save to file
		stbi_flip_vertically_on_write(true);
		if (stbi_write_png(outPath.c_str(), a_width, a_height, 3, (void*)dataBuffer, a_width * 3))
		{
			CL_CORE_INFO("{0} saved at {1}", a_imageName, GetWorkingDir());
		}

		// Free resources
		delete[] dataBuffer;
	}

	const char* Utility::GetGLErrorStr(unsigned int a_error)
	{
		switch (a_error)
		{
		case GL_NO_ERROR:          return "No error";
		case GL_INVALID_ENUM:      return "Invalid enum";
		case GL_INVALID_VALUE:     return "Invalid value";
		case GL_INVALID_OPERATION: return "Invalid operation";
		case GL_STACK_OVERFLOW:    return "Stack overflow";
		case GL_STACK_UNDERFLOW:   return "Stack underflow";
		case GL_OUT_OF_MEMORY:     return "Out of memory";
		default:                   return "Unknown error";
		}
	}
}