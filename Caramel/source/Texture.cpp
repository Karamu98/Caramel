#include "Texture.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include "Defines.h"
#include "gl_core_4_4.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "imgui.h"


Texture::Texture()
{
	m_textBuffer = new char[64]; /// :::OPTIMISE:::
}

Texture::~Texture()
{
	delete[] m_filePath;
}

/// <summary>
/// Loads in an image and sends it to the GPU
/// </summary>
/// <param name="a_filePath">The file path to the image, starting from executable directory.</param>
void Texture::LoadFromFile(std::string a_path)
{
	m_filePath = a_path.c_str();

	int width, height, channels;
	width = height = channels = 0;

	// Load the image in using stb_image
	unsigned char* data = stbi_load(m_filePath, &width, &height, &channels, 0);

	// Null checking
	if (data == nullptr)
	{
		CL_CORE_ERROR("Could not load: " + std::string(m_textBuffer) + ".");
		CL_CORE_ERROR("STB-> ",stbi_failure_reason());
		return;
	}

	// Pass texture to GPU
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// Set up the texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free loaded image
	stbi_image_free(data);

	CL_CORE_INFO("Image at " + std::string(m_textBuffer) + " loaded.");

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
		delete[] m_filePath;

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
		LoadFromFile(m_textBuffer);
	}

	ImGui::PopID();
}

