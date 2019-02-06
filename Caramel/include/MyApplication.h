#pragma once
#ifndef __Application_H_
#define __Application_H_

#include "Application.h"
#include "Scene.h"
#include <glm/glm.hpp>

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
	glm::mat4	m_cameraMatrix;
	glm::mat4	m_projectionMatrix;

	Scene m_scene;

	// UI Management
	// State Control
	bool bComponentTool;
	bool bComponentAdd;
};

#endif // __Application_H_