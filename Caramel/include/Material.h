#ifndef _MATERIAL_H__
#define _MATERIAL_H__

#include "glm/glm.hpp"
#include "Texture.h"

enum RendereringMode
{
	RENDER_OPAQUE,
	RENDER_CUTOUT,
	RENDER_FADE,
	RENDER_TRANSPARENT
};

class Material
{
public:
	Material();
	~Material();

	void OnGUI();

private:

	RendereringMode m_renderMode;
	Texture m_albedo;
	Texture m_metalic;
	Texture m_normal;
	Texture m_height;

	bool m_emission;


	/// Forward Rendering options
	bool m_specularHighlights;
	bool m_reflections;




};

#endif // !_MATERIAL_H__
