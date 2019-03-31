#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include "Light.h"


class PointLight : public Light
{
public:

	PointLight(Entity* a_pOwner);
	~PointLight();

	virtual void Update(float a_fDeltaTime) {};
	virtual void Draw(Shader* a_shader);
	virtual void OnGUI();

private:

	float m_constant;
	float m_linear;
	float m_quadratic;
	float m_radius;
};



#endif