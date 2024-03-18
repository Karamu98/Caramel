#pragma once

#include <Caramel.h>


class TestGameLayer : public Caramel::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnUpdate() override;

private:
	std::shared_ptr<Caramel::Texture> m_testTexture;
	std::shared_ptr<Caramel::Shader> m_testShader;

	std::shared_ptr<Caramel::VertexArray> m_testTriangle;
	std::shared_ptr<Caramel::VertexArray> m_testStar;
};