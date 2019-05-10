#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include "Light.h"


class PointLight : public Light
{
public:

	PointLight(Entity* a_pOwner);
	~PointLight();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Shader* a_shader, int a_number);
	virtual void OnGUI();

	virtual void PrePass(Shader* a_shader, int a_number);

private:

	float m_attenuation;
	float m_constant;
	float m_linear;
	float m_quadratic;
};



#endif