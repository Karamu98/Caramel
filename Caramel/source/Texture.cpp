#include "Texture.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include "Defines.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "imgui.h"
#include <string>


Texture::Texture()
{
}

Texture::~Texture()
{
}

/// <summary>
/// Loads in an image and sends it to the GPU
/// </summary>
/// <param name="a_filePath">The file path to the image, starting from executable directory.</param>
unsigned int Texture::TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		CL_CORE_INFO(std::string("Texture failed to load at path: " + filename + "."));
		stbi_image_free(data);
		return -1;
	}

	return textureID;
}

unsigned int Texture::GetTextureID()
{
	return m_textureID;
}

void Texture::OnGUI(std::string a_name)
{
}

void Texture::Unload()
{
	glDeleteTextures(1, &m_textureID);
}

