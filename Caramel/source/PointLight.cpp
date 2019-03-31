#include "PointLight.h"

PointLight::PointLight(Entity * a_pOwner) : Light(a_pOwner)
{
}

PointLight::~PointLight()
{
}

void PointLight::Draw(Shader * a_shader)
{
	// Pass base + pointlight here
}

void PointLight::OnGUI()
{
	// Expose colours and values from base and pointlight here
}
