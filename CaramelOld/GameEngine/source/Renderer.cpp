#include "Renderer.h"

#include "Utilities.h"
#include "gl_core_4_4.h"
#include "Material.h"
#include "Log.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Init(unsigned int a_programID, const char* a_vShaderPath, const char* a_fShaderPath)
{
	CL_CORE_INFO("Initialising Renderer.");

	m_programID = a_programID;
	//m_defaultMaterial.SetShader(Utility::loadShader(a_vShaderPath, GL_VERTEX_SHADER), Utility::loadShader(a_fShaderPath, GL_FRAGMENT_SHADER));
}

void Renderer::Draw(ModelComponent* a_model)
{
	// If model doesnt have a material, use m_defaultMaterial
	// Get model component (Position)



}
