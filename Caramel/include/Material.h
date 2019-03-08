#ifndef _MATERIAL_H__
#define _MATERIAL_H__

#include "glm/glm.hpp"
#include "Texture.h"

class Material
{
public:
	Material();
	~Material();

	void OnGUI();
	void Bind();

private:

	Texture m_albedo;
	Texture m_metalic;
	Texture m_normal;
	Texture m_height;
};

#endif // !_MATERIAL_H__
