#ifndef _Pyramid_H__
#define _Pyramid_H__

#include "Shape.h"

class Pyramid : public Shape
{
public:
	Pyramid();

protected:
	void DrawShape() override;

private:
	const static float vertexData[];
	const static unsigned int indiciesData[];

};

#endif