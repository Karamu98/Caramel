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
	void ImDraw() override;
	void Draw() override;
	void Destroy() override;

private:
	
	std::shared_ptr<Shape> InitShape();

private:	
	std::shared_ptr<Caramel::Framebuffer> m_viewFramebuffer;
	std::shared_ptr<Caramel::Shader> lightShader; // Debug shader to show light position
	std::shared_ptr<Shape> shape;
	std::shared_ptr<Cube> light;
	std::shared_ptr<Caramel::Camera> cam;
	glm::vec3 lightColour;

	// Cube "Material" to be abstracted
	std::shared_ptr<Caramel::Shader> simpleShader; // Universal shader for drawing Blinn-phong
	std::shared_ptr<Caramel::Texture> cubeDiffuse;
	std::shared_ptr<Caramel::Texture> cubeSpecular;
	std::shared_ptr<Caramel::Texture> cubeEmission;
	float cubeShine;
	float cubeBrightness;

	// Doesn't belong here, things like this should be in the renderer
	bool m_renderWireFrame;
	float m_gammaCorrection;

	// Input
	bool shouldScreenshot;

	// Cache
	glm::vec2 portSize;
};

#endif