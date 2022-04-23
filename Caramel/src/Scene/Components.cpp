#include "clpch.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>

#include "Scene/Components.h"
#include "Core/Resource/ModelResource.h"
#include "Core/Resource/MaterialResource.h"

/// THIS IS TO BE CHANGED

namespace Caramel::Components
{
	Transform::Transform()
	{
		m_matrix = glm::inverse(glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)));
	}

	Transform::Transform(glm::mat4x4 a_mat)
	{
		m_matrix = a_mat;
	}

	Transform::Transform(glm::vec3 a_pos)
	{
		m_matrix = glm::inverse(glm::lookAt(a_pos, glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)));
	}

	Transform::Transform(glm::vec3 a_pos, glm::vec3 a_forward)
	{
		m_matrix = glm::inverse(glm::lookAt(a_pos, glm::vec3(0, 0, 1), a_forward));
	}

	void Transform::SetPosition(glm::vec3 a_newPos)
	{
		m_matrix[3] = glm::vec4(a_newPos, 1);
	}

	void Transform::LookAt(glm::vec3 a_pos)
	{
		m_matrix = glm::lookAt(GetPos(), a_pos, glm::vec3(0, 1, 0));
	}

	void Transform::LookAt(Transform& a_pos)
	{
		m_matrix = glm::inverse(glm::lookAt(GetPos(), a_pos.GetPos(), glm::vec3(0, 1, 0)));
	}

	void Transform::Rotate(float a_angle, glm::vec3 a_axis)
	{
		m_matrix = glm::rotate(m_matrix, a_angle, a_axis);
	}

	void Transform::Scale(glm::vec3 a_newScale)
	{
		m_matrix[0] = glm::vec4(1, 0, 0, 0) * a_newScale.x;
		m_matrix[1] = glm::vec4(0, 1, 0, 0) * a_newScale.y;
		m_matrix[2] = glm::vec4(0, 0, 1, 0) * a_newScale.z;
	}


	Camera::Camera(Transform* transformComponent) : m_transform(transformComponent)
	{
	}

	void Camera::SetProjectionMatrix(glm::mat4 a_newMat)
	{
		m_projMatrix = a_newMat;
	}
	void Camera::SetRatio(unsigned int a_w, unsigned int a_h)
	{
		m_projMatrix = glm::perspective(glm::radians(50.0f), (float)a_w / (float)a_h, 0.1f, 5000.0f);
	}
	glm::mat4 Camera::GetProjectionMatrix()
	{
		return m_projMatrix;
	}
	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::inverse((*m_transform->GetMatrix()));
	}
	glm::mat4 Camera::GetProjectionView()
	{
		return m_projMatrix * GetViewMatrix();
	}

	void Camera::Draw(std::shared_ptr<Shader> a_program)
	{
		m_projViewMatrix = m_projMatrix * glm::inverse(*m_transform->GetMatrix());

		a_program->SetMat4("camProjView", m_projViewMatrix);
		a_program->SetVec3("gCamPos", m_transform->GetPos());
	}

	Model::Model(std::string path)
	{
	}

}