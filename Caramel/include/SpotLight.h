#ifndef __SPOT_LIGHT_H__
#define __SPOT_LIGHT_H__

#include "Light.h"

class SpotLight : public Light
{
public:

	SpotLight(Entity* a_pOwner);
	~SpotLight();

	virtual void Update(float a_fDeltaTime) {};
	virtual void Draw(Shader* a_shader, int a_number);
	virtual void OnGUI();

private:

	glm::vec3 m_direction;
	float m_cutoff, m_outerCutoff, m_constant, m_linear, m_quadratic;
};
#endif // !__SPOT_LIGHT_H__
