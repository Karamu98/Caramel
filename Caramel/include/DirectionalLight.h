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
	virtual Component* Duplicate(Entity* a_owner);

	virtual void Save(std::ostream* a_outStream);

	void PrePass(Shader* a_shader, glm::vec3 a_center, glm::vec2 a_number);
	void SetDirection(glm::vec3 a_newDir);

private:

	glm::vec3 m_direction;

	glm::mat4 m_lightProjection;
	glm::mat4 m_lightMatrix;
	glm::mat4 m_lightProjView;

	glm::vec2 m_atlasIndex;
};

#endif