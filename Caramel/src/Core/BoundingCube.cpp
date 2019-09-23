#include "clpch.h"
#include "BoundingCube.h"

namespace Caramel
{
	BoundingCube::BoundingCube() : m_minimum(-1.0f), m_maximum(1.0f)
	{
		CalculateExtends();
	}
	BoundingCube::BoundingCube(int a_width, int a_height, int a_depth) : m_extents(glm::vec3(a_width, a_height, a_depth))
	{
		CalculateBounds();
	}
	BoundingCube::BoundingCube(const glm::vec3& a_extents) : m_extents(a_extents)
	{
		CalculateBounds();
	}
	void BoundingCube::SetExtents(int a_width, int a_height, int a_depth)
	{
		m_extents = glm::vec3(a_width, a_height, a_depth);
		CalculateBounds();
	}
	void BoundingCube::SetExtents(const glm::vec3& a_bounds)
	{
		m_extents = a_bounds;
		CalculateBounds();
	}
	void BoundingCube::SetBounds(const glm::vec3& a_minPoint, const glm::vec3& a_maxPoint)
	{
		m_minimum = a_minPoint;
		m_maximum = a_maxPoint;
		CalculateExtends();
	}
	glm::vec3 BoundingCube::GetExtents()
	{
		return m_extents;
	}
	glm::vec3 BoundingCube::GetMinBound()
	{
		return m_minimum;
	}
	glm::vec3 BoundingCube::GetMaxBound()
	{
		return m_maximum;
	}
	void BoundingCube::CalculateBounds()
	{
		m_minimum = -(m_extents * 0.5f);
		m_maximum = m_extents * 0.5f;
	}
	void BoundingCube::CalculateExtends()
	{
		m_extents.x = m_maximum.x - m_minimum.x;
		m_extents.y = m_maximum.y - m_minimum.y;
		m_extents.z = m_maximum.z - m_minimum.z;
	}
}