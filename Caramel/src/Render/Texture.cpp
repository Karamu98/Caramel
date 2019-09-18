#include "clpch.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include "Core/Log.h"
#include "Core/Utilities.h"

namespace Caramel
{
	std::unordered_map<std::string, std::weak_ptr<Texture>> Texture::s_textureLibrary;

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
			GLint format = 0;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 2)
				format = GL_RG;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			CL_CORE_INFO("Loaded texture at: {0}", a_texturePath);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
		else
		{
			CL_CORE_ERROR("Failed to load texture at: {0}", a_texturePath);
			stbi_image_free(data);
			return false;
		}

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
		std::shared_ptr<Texture> newTexture;

		// If the texture is tracked
		if (s_textureLibrary.find(a_texturePath) != s_textureLibrary.end())
		{
			// And if the texture is still loaded, return the texture
			if (newTexture = s_textureLibrary[a_texturePath].lock())
			{
				return newTexture;
			}
		}

		newTexture = std::make_shared<Texture>();

		if (newTexture->LoadTexture(a_texturePath))
		{
			// Register the new texture
			s_textureLibrary[a_texturePath] = newTexture;
			return newTexture;
		}
		else
		{
			return nullptr;
		}
	}
}