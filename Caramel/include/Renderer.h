#ifndef _RENDERER_H__
#define _RENDERER_H__

#include "Shader.h"

class Scene;
class Camera;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(bool a_isDeferred); // Initialises the renderer, setting up what rendering mode we are using
	void Draw(Scene* a_sceneToRender); // Draws all of the objects in a scene using the current rendering mode
	void OnGUI();

private:

	/// CACHE
	Camera* activeCamera; // The active camera this frame.
};


#endif // !_RENDERER_H__

