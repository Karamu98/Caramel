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
	m_textBuffer = new char[64]; /// :::OPTIMISE:::
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

void Texture::SaveToMeta()
{
	std::ofstream file("textureTest.meta", std::ios::out|std::ios::binary);

	if (!file.is_open())
	{
		CL_CORE_ERROR("Unable to open textureTest.meta");
		return;
	}

	// Write the member variables to a file for easy reading on reload

	// If there is a file path
	if (m_filePath != nullptr)
	{
		int filePathSize = strlen(m_filePath);
		file << filePathSize;

		for (int i = 0; i < filePathSize; i++)
		{
			file << m_filePath[i]; // Add each character to the file
		}
	}
	else
	{
		file << FileTag::NO_MEMBER;
	}

	// Close the file
	file.close();
}

void Texture::LoadFromMeta(const char * a_filePathToMeta)
{
	// Read in meta data to member data
	std::ifstream file(a_filePathToMeta, std::ios::binary);

	if (!file.is_open())
	{
		CL_CORE_ERROR("Unable to open textureTest.meta");
		return;
	}

	uint8_t flagCheck = 0;

	file >> flagCheck;

	// Loading in texture path
	if (flagCheck != FileTag::NO_MEMBER)
	{
		// Go back a byte
		file.seekg(std::ios::cur - sizeof(FileTag::NO_MEMBER));

		// Read in the size of the path
		int filePathSize;
		file >> filePathSize;
		// Deallocate any space we maybe using and reallocate to size needed
		delete m_filePath;

		char* tempFilePath = new char[filePathSize];

		// Read in each character
		for (int i = 0; i < filePathSize; i++)
		{
			char chara;
			file >> chara;
			tempFilePath[i] = chara;
		}

		m_filePath = tempFilePath;
	}
	else
	{
		CL_CORE_INFO(a_filePathToMeta, ": has no filepath saved");
		m_filePath = nullptr;
	}

	file >> flagCheck;

	// Close the file after reading
	file.close();

}

unsigned int Texture::GetTextureID()
{
	return m_textureID;
}

void Texture::OnGUI(std::string a_name)
{
	std::string id = "Texture" + a_name;
	ImGui::PushID(id.c_str());
	ImGui::InputText("Image Path", m_textBuffer, sizeof(char) * 64);

	if (ImGui::Button("Load"))
	{
		
	}

	ImGui::PopID();
}

void Texture::Unload()
{
	glDeleteTextures(1, &m_textureID);
}

