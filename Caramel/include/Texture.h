#ifndef _TEXTURE_H__
#define _TEXTURE_H__

#include "glm/glm.hpp"
#include <vector>

class Texture
{
public:
	Texture();
	~Texture();

	void LoadFromFile(const char* a_filePath);
	void SaveToMeta();
	void LoadFromMeta(const char* a_filePathToMeta);
	unsigned int GetTextureID();
	float** GetOverrideColour();
	char* GetFilePath();
	void OnGUI(std::string a_name);

private:
	char* m_filePath; // Stores the original file location
	glm::vec4 m_overrideColour; // This is the colour to apply to the texture
	unsigned int m_textureID; // This stores the ID of the texture for later reference

	// ImGui
	float** m_colour;

};

#endif // !_TEXTURE_H__
