#pragma once

// Class declarations
struct GLFWwindow;

#define BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)



namespace Caramel
{
	class ModelResource;
	class Texture;

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
		static std::string GetEngineDir();

		// OpenGL
		static void GetGLErrors();
		static void Screenshot(const std::string& a_imageName, unsigned int a_width, unsigned int a_height);

		// ImGui helpers
		static bool TextureButton(const std::string& a_textureName, const std::shared_ptr<Texture>& a_texture);
		static bool ModelButton(const std::string& a_textureName, const std::shared_ptr<ModelResource>& a_texture);
		static bool SkyboxButton();
		static bool PathEntry(const std::string& a_fieldName, char* a_buffer, size_t a_length);


	private:
		static const char* GetGLErrorStr(unsigned int a_error);
		static std::string sm_workingDir; // Cache path
		static std::string sm_engineDir;  // Cache path

	};
}