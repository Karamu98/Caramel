#ifndef _RENDERER_H__
#define _RENDERER_H__

class Material;
class Scene;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(bool a_isDeferred); // Initialises the renderer, setting up what rendering mode we are using
	void Draw(Scene* a_sceneToRender); // Draws all of the object in a scene using the current rendering mode

private:

	void Render(Scene* a_sceneToRender);
	void ForwardRendering(Scene* a_sceneToRender);
	void DeferredRendering(Scene* a_sceneToRender);

	bool m_bDeferredRendering; // Are we rendering forward or deferred?
	Material* m_DefaultMaterial; // For any objects that do not have a material


	/// CACHE
	Camera* activeCamera; // The active camera this frame.
};


/*
============== Pipeline

		VERTEX: Positions objects in screen space from world space
		|
		GEOMETRY: Generate primitives
		|
		TESSALATION: Breaks triangles into more triangles, this is used to add detail with bump maps
		|
		FRAGMENT || DEFFERED	FRAG: This is used to colour the meshes using lighting and other objects || DEF: This will colour the objects and light them later
		|			|
		END			?
					|
					END


	Deferred Rendering: Splits the fragmentation pass into two. Performance becomes O(geometry + lights).
	Forward Rendering: Higher quality at the cost of performance, O(geometry * lights).
*/



#endif // !_RENDERER_H__

