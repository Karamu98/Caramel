#include "Texture.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include "Defines.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Texture::LoadImageFromFile(const char* a_filePath)
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

	float* parser;
	parser = (float*)data;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x+=channels)
		{
			// Initalise the vec4
			m_textureData.push_back(glm::vec4(-1, 1, -1, 0));

			

			int iter = (width * y) + x;
			m_textureData.at(iter).x = *parser;
			parser++;
			m_textureData.at(iter).y = *parser;
			parser++;
			m_textureData.at(iter).z = *parser;
			parser++;
			m_textureData.at(iter).w = *parser;
			parser++;
		}
	}


}

std::vector<glm::vec4>* Texture::GetImage()
{
	// If there is no override colour, just return texture data
	if (m_overrideColour == nullptr)
	{
		// If we have no texture loaded, log an error as it has been requested
		if (m_textureData.empty())
		{
			CL_CORE_ERROR("Texture has not been loaded.");
			return nullptr;
		}
		else 
		{
			return &m_textureData;
		}
	}
	else
	{
		// Create m_outputData
		// Iterate over all of m_textureData and apply colour, then set to m_outputData
		return nullptr; // Return 
	}
}

void Texture::SaveToFile()
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

	// If the texture data isn't empty
	if (!m_textureData.empty())
	{
		file << m_textureData.size();

		for (int i = 0; i < m_textureData.size; i++)
		{
			// Add each value of colour to file, TODO: override glm::vec4 '<<' and '>>'
			file << m_textureData.at(i).x;
			file << m_textureData.at(i).y;
			file << m_textureData.at(i).z;
			file << m_textureData.at(i).w;
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

void Texture::Load(const char * a_filePathToMeta)
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

	// Loading in texture data
	if (flagCheck != FileTag::NO_MEMBER)
	{
		// Go back a byte
		file.seekg(std::ios::cur - sizeof(FileTag::NO_MEMBER));

		// Read in the size of the data
		int texSize;
		file >> texSize;
		// Clear our data vector
		m_textureData.clear();

		// Read in each vector4
		for (int i = 0; i < texSize; i++)
		{
			float x, y, z, w;
			x = y = z = w = 0;
			file >> x;
			file >> y;
			file >> z;
			file >> w;

			m_textureData.push_back(glm::vec4(x, y, z, w));
		}

	}
	else
	{
		CL_CORE_INFO(a_filePathToMeta, ": has no texture data saved");
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
