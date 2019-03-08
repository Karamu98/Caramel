#include "Material.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "gl_core_4_4.h"

Material::Material()
{
	m_albedo = Texture();
	m_metalic = Texture();
	m_normal = Texture();
	m_height = Texture();
}

Material::~Material()
{

}

void Material::OnGUI()
{
	// Textures
	m_albedo.OnGUI("Albedo");
	m_metalic.OnGUI("Metalic");
	m_normal.OnGUI("Normal");
	m_height.OnGUI("Height");
}

void Material::Bind()
{
	glActiveTexture(GL_TEXTURE0 + 1);

}