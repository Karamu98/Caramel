#pragma once

#include <Caramel.h>

#include "FreeCamera.h"
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

	FreeCamera m_cam;
	std::vector<std::shared_ptr<TestModel>> m_models;
};