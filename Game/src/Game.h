#ifndef __GAME_H__
#define __GAME_H__
#include "Caramel.h"

#include <memory>
#include <glm/glm.hpp>

#include "FreeLookCam.h"

class Game : public Caramel::Application
{
public:
	Game();
	~Game();

	bool OnCreate() override;
	void Update(float deltaTime) override;
	void ImDraw() override;
	void Draw() override;
	void Destroy() override;

private:

	FreeLookCam m_freeCam;
	Caramel::Entity m_model;
};

#endif