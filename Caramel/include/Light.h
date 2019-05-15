#ifndef _LIGHT_H__
#define _LIGHT_H__

//includes
#include "Component.h"
#include "glm/glm.hpp"

class Entity;
class Shader;

class Light : public Component
{
public:

	Light(Entity* a_pOwner);
	~Light();

	virtual void Update(float a_fDeltaTime) {};
	virtual void Draw(Shader* a_shader, int a_number) = 0;
	virtual void OnGUI();
	virtual bool OnDelete();

	void SetDiffuse(glm::vec3 a_colour);
	void SetSpecular(glm::vec3 a_spec);

protected:

	glm::vec3 m_diffuseColour;
	glm::vec3 m_specularColour;

};



#endif