#ifndef _LIGHT_H__
#define _LIGHT_H__

//includes
#include "Component.h"
#include "glm/glm.hpp"

class Entity;

typedef enum LightType
{
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_DIRECTION
}LightType;

class Light : public Component
{
public:

	Light(Entity* a_pOwner);
	~Light();

	virtual void Update(float a_fDeltaTime) {};
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
	virtual void OnGUI();

private:

	LightType m_lightType;
	glm::vec4 m_colour;
	float m_intensity;

};



#endif