#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include "Light.h"
#include "Transform.h"

class Cube;


class PointLight : public Light
{
public:

	PointLight(Entity* a_pOwner);
	~PointLight();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Shader* a_shader, int a_number);
	virtual void OnGUI();
	virtual Component* Duplicate(Entity* a_owner);

	virtual void PrePass(Shader* a_shader, glm::vec2 a_number);
	void PostPass(Shader* a_shader); // Just used to show the representation light

private:

	float m_attenuation;
	float m_linear;
	float m_quadratic;
	glm::vec2 m_atlasIndex;

	Cube* m_cubeRepresentation;
	Transform m_lightTransform;
};



#endif