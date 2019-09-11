#include "clpch.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stdio.h>

#include <imgui.h>
#define IMGUI_IMPL_API
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Utilities.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "Core/Log.h"
#include "Render/Window.h"

#include <stb_image_write.h>

namespace Caramel
{
	static double s_prevTime = 0;
	static float s_totalTime = 0;
	static float s_deltaTime = 0;

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
		char path[MAX_PATH] = "";
		GetModuleFileNameA(NULL, path, MAX_PATH);

		std::string dir(path);
		dir = dir.substr(0, dir.find_last_of("\\/"));
		return dir + '\\';
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

	void Utility::TextureButton(const std::string& a_textureName, const std::shared_ptr<Caramel::Texture>& a_texture)
	{
		ImTextureID texID = (void*)(intptr_t)a_texture->GetID();

		if (ImGui::ImageButton(texID, { 80, 80 }, ImVec2(0, 1), ImVec2(1, 0)))
		{
			
			a_texture->Reload(Caramel::Utility::OpenFileDialog(AppWindow::GetNative()));
		}
		ImGui::SameLine();
		ImGui::Text(a_textureName.c_str());
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