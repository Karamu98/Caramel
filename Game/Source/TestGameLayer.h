#pragma once

#include <Caramel.h>


class TestGameLayer : public Caramel::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnImGuiRender() override;

	virtual void OnUpdate(Caramel::Timestep ts) override;

private:
	void SetPointOnCircle(float radius, float angleInDegrees);


	std::shared_ptr<Caramel::Texture> m_testTexture;
	std::shared_ptr<Caramel::Shader> m_test2DShader;

	std::shared_ptr<Caramel::VertexArray> m_testTriangle;
	std::shared_ptr<Caramel::VertexArray> m_testStar;

	std::shared_ptr<Caramel::Shader> m_test3DShader;
	std::shared_ptr<Caramel::VertexArray> m_testCube;
	std::shared_ptr<Caramel::VertexArray> m_testModel;

	float m_testOffset[2] = { 0.0f, 0.0f };

	float m_spinSpeed = 90.0f;
	float m_currrentAngle = 0.0f;
	float m_radius = 0.2f;
	bool m_spin = true;

	float m_cubeScale;
	float m_cubeScaleSpeed = 2.5f;

	float m_fov = 90.0f;
	float m_nearPlane = 1.0f;
	float m_farPlane = 10.0f;
};