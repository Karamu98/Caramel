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

	virtual void PrePass(Shader* a_shader, glm::vec2 a_number);

	void SetDirection(glm::vec3 a_newDir);

private:

	glm::vec3 m_direction;
	float m_cutoff, m_outerCutoff, m_linear, m_quadratic;
	glm::vec2 m_atlasIndex;

	glm::mat4 m_lightProjection;
	glm::mat4 m_lightMatrix;
	glm::mat4 m_lightProjView;
};
#endif // !__SPOT_LIGHT_H__
