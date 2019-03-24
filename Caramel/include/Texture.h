#ifndef _TEXTURE_H__
#define _TEXTURE_H__

#include "glm/glm.hpp"
#include <vector>

class Texture
{
public:
	Texture();
	~Texture();
	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);
	void SaveToMeta();
	void LoadFromMeta(const char* a_filePathToMeta);
	unsigned int GetTextureID();
	void OnGUI(std::string a_name);
	void Unload();

	std::string m_type; // Stores the type of texture this is, used with loading models with materials
	const char* m_filePath; // Stores the original file location
	unsigned int m_textureID; // This stores the ID of the texture for later reference

private:

	// ImGui
	char* m_textBuffer;

};

#endif // !_TEXTURE_H__
