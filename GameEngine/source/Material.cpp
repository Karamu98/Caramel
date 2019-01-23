#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::SetShader(static unsigned int a_vertexShader, static unsigned int a_fragmentShader)
{
	vertexShader = a_vertexShader;
	fragmentShader = a_fragmentShader;
}

unsigned int Material::GetVertexShader()
{
	return vertexShader;
}

unsigned int Material::GetFragmentShader()
{
	return fragmentShader;
}
