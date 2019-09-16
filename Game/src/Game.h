#ifndef __GAME_H__
#define __GAME_H__
#include "Caramel.h"

#include "Shapes/Pyramid.h"
#include "Shapes/Cube.h"
#include "Shapes/Sphere.h"

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
	std::shared_ptr<Caramel::Camera> cam;

	// Model with a transform
	std::shared_ptr<Caramel::Model> model;
	Caramel::Transform modelTransform;

	// Point Light
	std::shared_ptr<Cube> lightShape;
	glm::vec3 lightColour;
	float lightLinear, lightQuadratic, lightAttenuation;

	// Shape "Material" to be abstracted
	std::shared_ptr<Caramel::Shader> objectShader;
	std::shared_ptr<Caramel::Texture> shapeAlbedo;
	std::shared_ptr<Caramel::Texture> shapeNormal;
	std::shared_ptr<Caramel::Texture> shapeMetallic;
	std::shared_ptr<Caramel::Texture> shapeRoughness;
	std::shared_ptr<Caramel::Texture> shapeAO;

	// Doesn't belong here, things like this should be in the renderer
	bool m_renderWireFrame;
	float m_gammaCorrection;

	// Input
	bool shouldScreenshot;

	// Cache
	glm::vec2 portSize;

	// State
	bool isShapeScene;
};

#endif