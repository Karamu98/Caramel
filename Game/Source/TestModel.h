#pragma once
#include <Caramel.h>

class TestModel
{
public:
	std::shared_ptr<Caramel::Model> Model;

	void ImGui();
	void Render();

	glm::vec3 Position;
	glm::vec3 Rotation;
};

