#include <Caramel.h>
#include <iostream>

class MyGame : public Caramel::Application
{
public:
	void Run() override;
};

void MyGame::Run()
{
	while(true)
	{
	std::cout << "hey" <<std::endl;
	}
}

Caramel::Application* CreateApplication()
{
	return new MyGame();
}
