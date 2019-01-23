#ifndef _RENDERER_H__
#define _RENDERER_H__


class Material;
class ModelComponent;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(unsigned int a_programID, const char* a_vShaderPath = "shaders/vertexShader.glsl", const char* a_fShaderPath = "shaders/fragmentShader.glsl");
	void Draw(ModelComponent* a_model);


private:
	
	unsigned int m_programID;

	//static Material m_defaultMaterial;
};



#endif