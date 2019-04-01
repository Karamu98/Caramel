#ifndef _RENDERER_H__
#define _RENDERER_H__

#include "Shader.h"
#include <map>
#include "Plane.h"

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

	Shader* AddShader(const char* a_name, Shader* a_newShader);
	Shader* CreateShader(const char* a_name, const char* a_vertexPath, const char* a_fragPath, const char* a_geometryShader = nullptr, const char* a_tessalationShader = nullptr);
	Shader* GetShader(const char* a_name);

private:

	// For deferred rendering
	Shader* m_firstPass;
	Shader* m_secondLight;
	unsigned int m_firstPassBuffer, m_posBufferID, m_normBuffer, m_colandSpecID, m_depthBuffer;
	Plane m_defQuad;

	std::map<std::string, Shader*> m_shaders;
};


#endif // !_RENDERER_H__

