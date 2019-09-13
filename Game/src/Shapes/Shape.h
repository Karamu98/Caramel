#ifndef _SHAPE_H__
#define _SHAPE_H__

#include <glm/glm.hpp>
#include <memory>
#include "Render/Shader.h"

class Shape
{
public:
	Shape();
	~Shape();

	void Draw(std::shared_ptr<Caramel::Shader> a_program);
	void SetPosition(glm::vec3 a_newPos);
	void Rotate(float a_angle, glm::vec3 a_axis);
	void Scale(glm::vec3 a_newScale);
	inline glm::vec3 GetPos() { return m_objMatrix[3]; }
	inline glm::mat4x4* GetMatrix() { return &m_objMatrix; }

protected:

	virtual void DrawShape() = 0;

	unsigned int m_vbo, m_vao;
	unsigned int m_objMatLoc;
	glm::mat4x4 m_objMatrix;
};

#endif // !_SHAPE_H__
