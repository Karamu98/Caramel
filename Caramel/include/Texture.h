#ifndef _TEXTURE_H__
#define _TEXTURE_H__

#include "glm/glm.hpp"
#include <vector>

class Texture
{
public:
	Texture();
	~Texture();

	void LoadFromFile(std::string a_path);
	void SaveToMeta();
	void LoadFromMeta(const char* a_filePathToMeta);
	unsigned int GetTextureID();
	void OnGUI(std::string a_name);

private:
	const char* m_filePath; // Stores the original file location
	glm::vec4 m_overrideColour; // This is the colour to apply to the texture
	unsigned int m_textureID; // This stores the ID of the texture for later reference

	// ImGui
	float** m_colour;
	char* m_textBuffer;

};

#endif // !_TEXTURE_H__
