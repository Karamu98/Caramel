#pragma once

#include <glm/glm.hpp>

#include "Transform.h"

namespace Caramel
{
	class BoundingCube
	{
	public:
		BoundingCube();
		BoundingCube(int a_width, int a_height, int a_depth);
		BoundingCube(const glm::vec3& a_extents);

		void SetExtents(int a_width, int a_height, int a_depth);
		void SetExtents(const glm::vec3& a_bounds);
		void SetBounds(const glm::vec3& a_minPoint, const glm::vec3& a_maxPoint);

		glm::vec3 GetExtents();
		glm::vec3 GetMinBound();
		glm::vec3 GetMaxBound();

	private:
		void CalculateBounds(); // Calculates the minimum and maximum points
		void CalculateExtends(); // Calculates the size of the bounding cube

	private:
		glm::vec3 m_extents;
		glm::vec3 m_minimum;
		glm::vec3 m_maximum;
	};
}
