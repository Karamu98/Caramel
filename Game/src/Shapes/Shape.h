#ifndef _SHAPE_H__
#define _SHAPE_H__

#include <glm/glm.hpp>
#include <memory>
#include "Render/Shader.h"
#include "Transform.h"

class Shape
{
public:
	Shape();
	~Shape();

	void Draw(std::shared_ptr<Caramel::Shader> a_program);
	Caramel::Transform* Transform();


protected:

	virtual void DrawShape() = 0;

	unsigned int m_vbo, m_vao;
	unsigned int m_objMatLoc;
	Caramel::Transform m_Transform;
};

#endif // !_SHAPE_H__
