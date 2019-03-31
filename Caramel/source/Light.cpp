#include "Light.h"
#include "gl_core_4_4.h"
#include "glm/ext.hpp"
#include "Entity.h"
#include "Shader.h"
#include "imgui.h"

typedef Component PARENT;

Light::Light(Entity * a_pOwner) : PARENT(a_pOwner)
{
}

Light::~Light()
{
}