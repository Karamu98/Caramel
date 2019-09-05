#include "clpch.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include "Core/Log.h"

namespace Caramel
{
	Texture::Texture() : m_isActive(false)
	{
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_textureID);
	}

	bool Texture::Bind(unsigned int a_texID)
	{
		if (m_isActive)
		{
			glActiveTexture(a_texID);
			glBindTexture(GL_TEXTURE_2D, m_textureID);
			return true;
		}
		return false;
	}

	void Texture::Reload(std::string a_newPath)
	{
		glDeleteTextures(1, &m_textureID);

		LoadTexture(a_newPath);
	}

	bool Texture::LoadTexture(const std::string& a_texturePath)
	{
		m_isActive = false;
		glGenTextures(1, &m_textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// Load image
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(a_texturePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			CL_CORE_INFO("Loaded texture at: {0}", a_texturePath);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			CL_CORE_INFO("Failed to load texture at: {0}", a_texturePath);
			stbi_image_free(data);
			return false;
		}
		stbi_image_free(data);

		m_filePath = a_texturePath;

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);
		m_isActive = true;
		return true;
	}

	std::shared_ptr<Texture> Texture::CreateTexture(const std::string& a_texturePath)
	{
		std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();

		if (newTexture->LoadTexture(Utility::GetWorkingDir() + a_texturePath))
		{
			return newTexture;
		}
		else
		{
			return nullptr;
		}
	}
}