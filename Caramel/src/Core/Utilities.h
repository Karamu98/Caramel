#pragma once

// Class declarations
class GLFWwindow;

#include "Render/Texture.h"

namespace Caramel
{
	// A utility class with static helper methods
	class Utility
	{
	public:

		// Utilities for timing; Get() updates timers and returns time since last Get call
		static void		ResetTimer();
		static float	TickTimer();
		static float	GetDeltaTime();
		static float	GetTotalTime();

		static std::string OpenFileDialog(GLFWwindow* a_window, const std::string& a_filter = "");
		static std::shared_ptr<std::string> ReadFile(const std::string& a_file);
		static std::string GetWorkingDir();

		// OpenGL
		static void GetGLErrors();
		static void Screenshot(const std::string& a_imageName, unsigned int a_width, unsigned int a_height);

		// ImGui helpers
		static void TextureButton(const std::string& a_textureName, const std::shared_ptr<Caramel::Texture>& a_texture);


	private:
		static const char* GetGLErrorStr(unsigned int a_error);

	};
}