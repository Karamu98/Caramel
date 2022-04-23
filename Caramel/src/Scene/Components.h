#pragma once

#include <glm/glm.hpp>

#include "Render/Shader.h"
/// <summary>
/// TODO: Make components data only
/// </summary>
/// 

namespace Caramel::Components
{
	struct EntityName
	{
		std::string Name;
	};

	class Transform
	{
	public:
		Transform();
		Transform(glm::mat4x4 a_mat);
		Transform(glm::vec3 a_pos);
		Transform(glm::vec3 a_pos, glm::vec3 a_forward);

		// Make this data only
		void SetPosition(glm::vec3 a_newPos);
		void LookAt(glm::vec3 a_pos);
		void LookAt(Transform& a_pos);
		void Rotate(float a_angle, glm::vec3 a_axis);
		void Scale(glm::vec3 a_newScale);
		inline glm::vec3 GetPos() { return m_matrix[3]; }
		inline glm::mat4x4* GetMatrix() { return &m_matrix; }

	private:
		glm::mat4x4 m_matrix;
	};

	class Camera
	{
	public:
		Camera(Transform* transformComponent);

		void Draw(std::shared_ptr<Shader> a_program);
		void SetProjectionMatrix(glm::mat4 a_newMat);
		void SetRatio(unsigned int a_w, unsigned int a_h);
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionView();

	protected:

		Transform* m_transform;
		glm::mat4 m_projMatrix;
		glm::mat4 m_projViewMatrix;
	};

	struct Model
	{
	public:
		Model(std::string modelPath);

		std::shared_ptr<class ModelResource> ModelResource;
	};
}