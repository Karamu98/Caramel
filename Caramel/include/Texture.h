#ifndef _TEXTURE_H__
#define _TEXTURE_H__

#include "glm/glm.hpp"
#include <vector>

class Texture
{
public:
	void LoadImageFromFile(const char* a_filePath);
	std::vector<glm::vec4>* GetImage();
	void SaveToFile();
	void Load(const char* a_filePathToMeta);

private:
	char* m_filePath; // Stores the original file location
	std::vector<glm::vec4> m_textureData; // This stores the colours of the input file
	std::vector<glm::vec4> m_outputData; // This is the final output after override colour
	glm::vec4* m_overrideColour; // This is the colour to apply to the texture

};

#endif // !_TEXTURE_H__
