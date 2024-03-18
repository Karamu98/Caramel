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
	std::shared_ptr<Caramel::Shader> m_testShader;

	std::shared_ptr<Caramel::VertexArray> m_testTriangle;
	std::shared_ptr<Caramel::VertexArray> m_testStar;

	float m_testOffset[2] = { 0.0f, 0.0f };

	float m_spinSpeed = 90.0f;
	float m_currrentAngle = 0.0f;
	float m_radius = 0.2f;
	bool m_spin = true;
};