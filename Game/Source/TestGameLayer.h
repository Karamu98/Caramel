#pragma once

#include <Caramel.h>

class TestGameLayer : public Caramel::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnImGuiRender() override;

	virtual void OnUpdate(Caramel::Timestep ts) override;


	std::shared_ptr<Caramel::Shader> m_test3DShader;
	std::shared_ptr<Caramel::Model> m_testModel;
	Caramel::Camera m_testCamera;

	float m_curModelSpin;
	float m_modelSpinSpeed = 0.5f;

	glm::vec3 m_modelPosition;
	glm::vec3 m_modelRotation;
};