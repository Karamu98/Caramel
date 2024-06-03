#pragma once

#include <Caramel.h>

#include "TestModel.h"

class TestGameLayer : public Caramel::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnImGuiRender() override;
	virtual void OnUpdate(Caramel::Timestep ts) override;

	void Spawn();

	int m_countToSpawn = 5;
	float m_seperation = 10.0f;

	Caramel::Scene m_scene;
	std::vector<Caramel::Entity> m_models;
};