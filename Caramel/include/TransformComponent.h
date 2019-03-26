#ifndef _TRANSFORM_COMPONENT_H_
#define _TRANSFORM_COMPONENT_H_

// This component is the ROOT component

//includes
#include "Component.h"
#include "glm/ext.hpp"

class Entity;


enum MATRIX_ROW
{
	RIGHT_VECTOR,
	UP_VECTOR,
	FORWARD_VECTOR,
	POSITION_VECTOR,
};

class TransformComponent : public Component
{
public:

	TransformComponent(Entity* a_pOwner);
	~TransformComponent();

	virtual void Update(float a_fDeltaTime) {};
	virtual void OnGUI();
	virtual bool OnDelete();


	glm::vec3 GetRightDirection();
	glm::vec3 GetUpDirection();
	glm::vec3 GetFacingDirection();
	glm::vec3 GetCurrentPosition();
	glm::mat4* pGetTransformMatrix();

	void SetRightDirection(glm::vec3 vRightDirection);
	void SetUpDirection(glm::vec3 vUpDirection);
	void SetFacingDirection(glm::vec3 vFacingDirection);
	void SetCurrentPosition(glm::vec3 vCurrentPosition);
	void SetScale(glm::vec3 a_newScale);

private:

	void SetEntityMatrixRow(MATRIX_ROW eRow, glm::vec3 vVec);
	glm::vec3 GetEntityMatrixRow(MATRIX_ROW eRow);
	glm::mat4 m_m4TransformMatrix;
};

#endif