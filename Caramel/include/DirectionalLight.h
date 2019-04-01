#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "Light.h"

class DirectionalLight : public Light
{
public:

	DirectionalLight(Entity* a_pOwner);
	~DirectionalLight();

	virtual void Update(float a_fDeltaTime) {};
	virtual void Draw(Shader* a_shader, int a_number);
	virtual void OnGUI();

private:

	glm::vec3 m_direction;
};

#endif