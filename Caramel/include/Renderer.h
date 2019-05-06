#ifndef _RENDERER_H__
#define _RENDERER_H__

#include "Shader.h"
#include <map>
#include "Plane.h"

class Scene;
class Camera;

typedef enum RenderingMode
{
	FORWARD,
	DEFERRED,
	FORWARDPLUS
}RenderingMode;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(RenderingMode a_renderMode); // Initialises the renderer, setting up what rendering mode we are using
	void Draw(Scene* a_sceneToRender); // Draws all of the objects in a scene using the current rendering mode
	void OnGUI();

	void ChangeRenderMode(RenderingMode a_newMode);

private:

	void InitForwardRendering();
	void InitDeferredRendering();
	void InitForwardPlusRendering();

	void DisableForward();
	void DisableDeferred();
	void DisableForwardPlus();

	void ForwardPass(Scene* a_scene, Camera* a_activeCam);
	void DeferredPass(Scene* a_scene, Camera* a_activeCam);
	void ForwardPlusPass(Scene* a_scene, Camera* a_activeCam);


	RenderingMode m_currentMode;

	unsigned int m_finalFramebuffer, m_finalColour, m_finalDepth;

	// For deferred rendering
	Shader* m_defGeo, *m_defGeoTransp, *m_defGeoAnim, *m_defGeoTranspAnim;
	Shader* m_defLight;
	unsigned int m_defGeoBuffer, m_posBuffer, m_normBuffer, m_albedoBuffer, m_specBuffer, m_rboDepth;
	Plane* m_defQuad;

	bool m_renderWireframe;
};


#endif // !_RENDERER_H__

