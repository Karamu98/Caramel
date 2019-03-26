#ifndef _TRANSFORM_H__
#define _TRANSFORM_H__

#include "glm/ext.hpp"
#include <vector>


enum MATRIX_ROW
{
	RIGHT_VECTOR,
	UP_VECTOR,
	FORWARD_VECTOR,
	POSITION_VECTOR,
};


class Transform
{
public:

	Transform();
	~Transform();

	glm::vec3 GetForward();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::vec3 GetPosition();
	glm::mat4* GetMatrix();

	void SetPosition(glm::vec3 a_newPos);
	void SetScale(glm::vec3 a_scale);
	void LookAt(glm::vec3 a_positionToLookAt);
	void SetParent(Transform* a_parent);
	void AddChild(Transform* a_newChild);
	void Reset();

private:

	void SetEntityMatrixRow(MATRIX_ROW eRow, glm::vec3 vVec);
	glm::vec3 GetEntityMatrixRow(MATRIX_ROW eRow);
	void UpdateChildren();

	glm::mat4 m_transformMatrix;
	Transform* m_parent;
	std::vector<Transform*> m_children;

};


#endif