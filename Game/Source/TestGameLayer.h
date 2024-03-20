#pragma once

#include <Caramel.h>


class TestGameLayer : public Caramel::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnImGuiRender() override;

	virtual void OnUpdate(Caramel::Timestep ts) override;


	std::shared_ptr<Caramel::Shader> m_test3DShader;
	std::vector<std::shared_ptr<Caramel::VertexArray>> m_testMeshes;

	float m_curModelSpin;
	float m_modelSpinSpeed = 0.5f;

	float m_fov = 90.0f;
	float m_nearPlane = 0.1f;
	float m_farPlane = 500.0f;
};