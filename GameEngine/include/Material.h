#ifndef _MATERIAL_H__
#define _MATERIAL_H__

class Material
{
public:
	Material();
	~Material();

	void SetShader(static unsigned int a_vertexShader, static unsigned int a_fragmentShader);

	unsigned int GetVertexShader();
	unsigned int GetFragmentShader();

private:
	unsigned int vertexShader;
	unsigned int fragmentShader;

	// Texture etc
};

#endif
