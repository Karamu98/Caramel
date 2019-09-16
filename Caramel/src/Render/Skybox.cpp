#include "clpch.h"
#include "Render/Skybox.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Core/Log.h"
#include "Core/Utilities.h"
#include "Render/Shader.h"

namespace Caramel
{
	std::shared_ptr<Shader> Skybox::sm_shader;
	std::shared_ptr<Skybox> Skybox::sm_instance;

	void Skybox::SetSkybox(const std::vector<std::string>& a_faces)
	{
		// If our instance does not exist, create it and load the shader
		if (!sm_instance)
		{
			sm_instance = std::make_shared<Skybox>();
			sm_shader = Shader::CreateShader(Utility::GetWorkingDir() + "resources/engine/shaders/skybox.glsl");

			const float skyboxVertices[] = {
				// positions          
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};

			// Create the cube
			glGenVertexArrays(1, &sm_instance->m_vao);
			glGenBuffers(1, &sm_instance->m_vbo);
			glBindVertexArray(sm_instance->m_vao);
			glBindBuffer(GL_ARRAY_BUFFER, sm_instance->m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			if (sm_shader->Bind())
			{
				sm_shader->SetInt("skybox", 0);
			}
			else
			{
				CL_CORE_ERROR("Skybox shader failure");
			}
		}

		// If we already have a texture loaded, delete it
		if (sm_instance->m_isValid)
		{
			glDeleteTextures(1, &sm_instance->m_textureID);
		}

		// Flag invalid while we load a new texture
		sm_instance->m_isValid = false;

		glGenTextures(1, &sm_instance->m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, sm_instance->m_textureID);

		// Loop through all our textures and load them in a cubemap
		int width, height, nrChannels;
		for (unsigned int i = 0; i < a_faces.size(); i++)
		{
			stbi_set_flip_vertically_on_load(false);
			unsigned char* data = stbi_load(a_faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				CL_CORE_ERROR("Cubemap texture failed to load at path: {0}", a_faces[i]);
				stbi_image_free(data);
				glDeleteTextures(1, &sm_instance->m_textureID);
				return;
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Now we have a texture loaded successfully, flag valid
		sm_instance->m_isValid = true;
	}

	void Skybox::Draw(Camera& a_camera)
	{
		if (!sm_instance || !sm_instance->m_isValid)
			return;

		if (sm_shader->Bind())
		{
			glm::mat4 view = glm::mat4(glm::mat3(a_camera.GetViewMatrix()));

			// Change depth function so depth test passes when values are equal to depth buffer's content
			glDepthFunc(GL_LEQUAL);  
			sm_shader->SetMat4("view", view);
			sm_shader->SetMat4("proj", a_camera.GetProjectionMatrix());

			// Skybox cube
			glBindVertexArray(sm_instance->m_vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, sm_instance->m_textureID);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS); // Set depth function back to default
		}
	}

	Skybox::Skybox()
	{
		CL_CORE_INFO("Skybox created");
	}

	Skybox::~Skybox()
	{
		if (m_isValid)
		{
			glDeleteTextures(1, &m_textureID);
		}
	}
}