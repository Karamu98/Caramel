#include "Transform.h"
#include "Log.h"

Transform::Transform()
{
	SetEntityMatrixRow(RIGHT_VECTOR, glm::vec3(1, 0, 0));
	SetEntityMatrixRow(UP_VECTOR, glm::vec3(0, 1, 0));
	SetEntityMatrixRow(FORWARD_VECTOR, glm::vec3(0, 0, 1));
	SetEntityMatrixRow(POSITION_VECTOR, glm::vec3(0, 0, 0));
}

Transform::~Transform()
{
}

glm::vec3 Transform::GetForward()
{
	return GetEntityMatrixRow(FORWARD_VECTOR);
}

glm::vec3 Transform::GetRight()
{
	return GetEntityMatrixRow(RIGHT_VECTOR);
}

glm::vec3 Transform::GetUp()
{
	return GetEntityMatrixRow(UP_VECTOR);
}

glm::vec3 Transform::GetPosition()
{
	if (m_parent != nullptr)
	{
		return m_parent->m_transformMatrix[3] + m_transformMatrix[3]; // Root pos + local pos
	}
	else
	{
		return GetEntityMatrixRow(POSITION_VECTOR);
	} 
}

glm::mat4 * Transform::GetMatrix()
{
	return &m_transformMatrix;
}

void Transform::SetPosition(glm::vec3 a_newPos)
{
	SetEntityMatrixRow(POSITION_VECTOR, a_newPos);
}

void Transform::SetScale(glm::vec3 a_scale)
{
	SetEntityMatrixRow(UP_VECTOR, GetEntityMatrixRow(UP_VECTOR) * a_scale.y);
	SetEntityMatrixRow(RIGHT_VECTOR, GetEntityMatrixRow(RIGHT_VECTOR) * a_scale.x);
	SetEntityMatrixRow(FORWARD_VECTOR, GetEntityMatrixRow(FORWARD_VECTOR) * a_scale.z);
}

void Transform::LookAt(glm::vec3 a_positionToLookAt)
{
	m_transformMatrix = glm::inverse(glm::lookAt(GetEntityMatrixRow(POSITION_VECTOR), a_positionToLookAt, glm::vec3(0, 1, 0)));
}

void Transform::SetParent(Transform * a_parent)
{
	if (a_parent == nullptr)
	{
		CL_CORE_ERROR("Parent is null");
		return;
	}
	m_parent = a_parent;
}

void Transform::AddChild(Transform* a_newChild)
{
	m_children.push_back(a_newChild);
}

void Transform::Reset()
{
	SetEntityMatrixRow(RIGHT_VECTOR, glm::vec3(1, 0, 0));
	SetEntityMatrixRow(UP_VECTOR, glm::vec3(0, 1, 0));
	SetEntityMatrixRow(FORWARD_VECTOR, glm::vec3(0, 0, 1));
	SetEntityMatrixRow(POSITION_VECTOR, glm::vec3(0, 0, 0));
}

void Transform::SetEntityMatrixRow(MATRIX_ROW eRow, glm::vec3 vVec)
{
	m_transformMatrix[eRow] = glm::vec4(vVec, (eRow == POSITION_VECTOR ? 1.f : 0.f));
}

glm::vec3 Transform::GetEntityMatrixRow(MATRIX_ROW eRow)
{
	return m_transformMatrix[eRow];
}

void Transform::UpdateChildren()
{
}
