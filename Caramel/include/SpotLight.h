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
	virtual Component* Duplicate(Entity* a_owner);
	virtual void Save(std::ostream* a_outStream);

	virtual void PrePass(Shader* a_shader, glm::vec2 a_number);

	void SetDirection(glm::vec3 a_newDir);

private:

	glm::vec3 m_direction;
	float m_cutoff, m_outerCutoff, m_linear, m_quadratic, m_attenuation;
	glm::vec2 m_atlasIndex;
};
#endif // !__SPOT_LIGHT_H__
