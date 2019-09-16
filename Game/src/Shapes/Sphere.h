#ifndef _SPHERE_H__
#define _SPHERE_H__

#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();
	~Sphere();

protected:
	void DrawShape() override;

private:
	unsigned int m_ibo, m_indiciesCount;
};

#endif