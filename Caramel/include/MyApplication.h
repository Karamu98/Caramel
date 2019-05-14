#ifndef __Application_H_
#define __Application_H_

#include "Application.h"
#include <glm/glm.hpp>
#include "Renderer.h"
#include "Scene.h"


class Camera;


// Derived application class that wraps up all globals neatly
class MyApplication : public Application
{
public:

	MyApplication();
	virtual ~MyApplication();

protected:

	virtual bool onCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();


private:

	// For the "Editor" camera
	Camera* m_activeCam;

	Scene m_scene;
	Renderer m_renderer;

	// UI Management
	// State Control
	bool bComponentTool;
	bool bComponentAdd;
	bool m_showUI;
};

#endif // __Application_H_