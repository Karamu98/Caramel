#ifndef __GAME_H__
#define __GAME_H__
#include "Caramel.h"

#include "Cube.h"
#include "Pyramid.h"

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

private:
	bool isWireframe;
	float specularAmount;
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
	std::shared_ptr<Caramel::Texture> newTexture;
	glm::vec3 lightColour;

	// Cache
	unsigned int shaderProgram, lightProgram;
	bool screenshot;
	glm::vec2 viewPortSize;
};

#endif