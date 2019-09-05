#ifndef _SQUARE_H__
#define _SQUARE_H__

#include "Shape.h"

class Cube : public Shape
{
public:
	Cube();

protected:
	void DrawShape() override;

private:
	const static float vertexData[];
	const static unsigned int indiciesData[];

};

#endif