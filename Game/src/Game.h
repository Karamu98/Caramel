#ifndef __GAME_H__
#define __GAME_H__
#include "Caramel.h"

#include "Shapes/Cube.h"
#include "Shapes/Pyramid.h"

#include <memory>
#include <glm/glm.hpp>

class Game : public Caramel::Application
{
public:
	Game();
	~Game();

	bool OnCreate() override;
	void Update(float a_deltaTime) override;
	void Draw() override;
	void Destroy() override;

private:
	void ImGuiDraw();
	void ResizeFBO(float a_width, float a_height);
	void Screenshot();
	std::shared_ptr<Shape> InitShape();
	void GUITextureButton(const std::string& a_textureName, const std::shared_ptr<Caramel::Texture>& a_texture);

private:
	bool isWireframe;
	float gammaCorrection;

	unsigned int defaultFBO;
	unsigned int defaultColourTex;
	unsigned int depthRender;
	float fboWidth, fboHeight;
	
	std::shared_ptr<Caramel::Shader> simpleShader;
	std::shared_ptr<Caramel::Shader> lightShader;
	std::shared_ptr<Shape> shape;
	std::shared_ptr<Cube> light;
	std::shared_ptr<Caramel::Camera> cam;
	glm::vec3 lightColour;

	std::shared_ptr<Caramel::Texture> cubeDiffuse;
	std::shared_ptr<Caramel::Texture> cubeSpecular;
	std::shared_ptr<Caramel::Texture> cubeEmission;
	float cubeShine;
	float cubeBrightness;

	// Cache
	unsigned int shaderProgram, lightProgram;
	bool screenshot;
	glm::vec2 viewPortSize;
};

#endif