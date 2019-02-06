#include "Light.h"
#include "gl_core_4_4.h"
#include "glm/ext.hpp"
#include "Entity.h"
#include "TransformComponent.h"

typedef Component PARENT;

Light::Light(Entity * a_pOwner) : PARENT(a_pOwner)
{
	SetComponentType(LIGHT);
}

Light::~Light()
{
}

void Light::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
	glm::vec3 pos = pGetOwnerEntity()->pGetRootTransformComp()->GetCurrentPosition();
	glUniform3fv(glGetUniformLocation(a_uProgramID, "LightPos"), 1, glm::value_ptr(pos));
}

void Light::OnGUI()
{
}