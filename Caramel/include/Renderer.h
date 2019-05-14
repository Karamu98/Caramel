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

	void InitForwardRendering() {};
	void InitDeferredRendering();
	void InitForwardPlusRendering() {};

	void DisableForward() {};
	void DisableDeferred();
	void DisableForwardPlus() {};

	void ForwardPass(Scene* a_scene, Camera* a_activeCam) {};
	void DeferredPass(Scene* a_scene, Camera* a_activeCam);
	void ForwardPlusPass(Scene* a_scene, Camera* a_activeCam) {};


	RenderingMode m_currentMode; // To switch render modes ::Incomplete::

	// Final buffers for FX
	unsigned int m_finalFramebuffer, m_finalColour, m_bloomColour, m_finalDepth;

	// For deferred rendering
	Shader* m_defGeo, *m_defGeoAnim; // Geometry shaders
	Shader* m_dirPrePass, *m_spotPrePass, *m_pointPrePass; // Light pre-pass shaders
	Shader* m_defLight; // Light shaders

	unsigned int m_defGeoBuffer, m_posBuffer, m_normBuffer, m_albedoBuffer, m_specBuffer, m_rboDepth; // Deferred buffers
	Plane* m_defQuad; // Fullscreen render quad

	// SSBOs
	unsigned int m_dirSSBO, m_pointSSBO, m_spotSSBO;

	// Shadows
	unsigned int m_shadowFBO;
	unsigned int m_shadowDepthTex;
	unsigned int m_shadowBufSize; // The current total w/h of the buffer
	unsigned int m_shadTexRes; // The resolution of each shadow map

	// HDR & Bloom
	float m_bloomMinimum;
	float m_gammaCorrection;


	bool m_renderWireframe;
};


#endif // !_RENDERER_H__

