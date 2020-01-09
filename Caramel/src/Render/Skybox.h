#pragma once

#include "Render/Shader.h"
#include <glm/glm.hpp>
#include "Render/Camera.h"
namespace Caramel
{
	class Skybox
	{
	public:
		/// <summary>
		/// Constructs a skybox.
		/// Faces must be in the following order: right, left, top, bottom, front and back
		/// </summary>
		static void SetSkybox(const std::vector<std::string>& a_faces);
		/// <summary>
		/// Draws the skybox.
		/// </summary>
		static void Draw(Camera& a_camera);
	private:
		static std::shared_ptr<Skybox> sm_instance;
		static std::shared_ptr<Shader> sm_shader;

	public:
		Skybox();
		~Skybox();
	private:
		unsigned int m_vao, m_vbo;
		unsigned int m_textureID;
		bool m_isValid;
	};
}