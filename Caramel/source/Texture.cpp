#include "Texture.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include "Defines.h"
#include "gl_core_4_4.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


/// <summary>
/// Loads in an image and sends it to the GPU
/// </summary>
/// <param name="a_filePath">The file path to the image, starting from executable directory.</param>
void Texture::LoadFromFile(const char* a_filePath)
{
	// TODO: Load in an image using STB image, help can be found on moodle

	int width, height, channels;
	width = height = channels = 0;

	// Load the image in using stb_image
	unsigned char* data = stbi_load(a_filePath, &width, &height, &channels, 0);

	// Null checking
	if (data == nullptr)
	{
		CL_CORE_ERROR("Could not load: ", a_filePath, ".");
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

	// Output the override colour
	if (m_overrideColour != nullptr)
	{
		file << m_overrideColour->x;
		file << m_overrideColour->y;
		file << m_overrideColour->z;
		file << m_overrideColour->w;
	}
	else
	{
		file << FileTag::NO_MEMBER;
	}

	if (sizeof(file) <= 0)
	{
		CL_CORE_ERROR("No data to save");
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
		m_filePath = new char[filePathSize];

		// Read in each character
		for (int i = 0; i < filePathSize; i++)
		{
			char chara;
			file >> chara;
			m_filePath[i] = chara;
		}
	}
	else
	{
		CL_CORE_INFO(a_filePathToMeta, ": has no filepath saved");
		m_filePath = nullptr;
	}

	file >> flagCheck;

	// Loading in override colour
	if (flagCheck != FileTag::NO_MEMBER)
	{
		// Go back a byte
		file.seekg(std::ios::cur - sizeof(FileTag::NO_MEMBER));

		// Deallocate our member
		delete m_overrideColour;

		// Read in vector4
		float x, y, z, w;
		x = y = z = w = 0;
		file >> x;
		file >> y;
		file >> z;
		file >> w;
		
		m_overrideColour = new glm::vec4(x, y, z, w);

	}
	else
	{
		CL_CORE_INFO(a_filePathToMeta, ": has no override colour saved");
		m_overrideColour = nullptr;
	}

	// Close the file after reading
	file.close();

}

unsigned int Texture::GetTextureID()
{
	return m_textureID;
}
