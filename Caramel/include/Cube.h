#ifndef _PRI_CUBE_H__
#define _PRI_CUBE_H__

#include "Transform.h"

class Shader;

class Cube
{
public:
	Cube();
	~Cube();

	void RenderCube(Shader* a_shader);

private:

	unsigned int m_VAO, m_VBO, m_IBO;

};

#endif // !_CUBE_H__
