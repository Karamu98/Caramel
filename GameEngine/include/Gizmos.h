#ifndef __GIZMOS_H_
#define __GIZMOS_H_

#include <glm/glm.hpp>

class Gizmos
{
public:

	static void		create(unsigned int a_maxLines = 32728, unsigned int a_maxTris = 32728);
	static void		destroy();

	// removes all Gizmos
	static void		clear();

	// draws current Gizmo buffers, either using a combined (projection * view) matrix, or separate matrices
	static void		draw(const glm::mat4& a_projectionView);
	static void		draw(const glm::mat4& a_view, const glm::mat4& a_projection);

	// Adds a single debug line
	static void		addLine(const glm::vec3& a_rv0,  const glm::vec3& a_rv1, 
							const glm::vec4& a_colour);

	// Adds a single debug line
	static void		addLine(const glm::vec3& a_rv0, const glm::vec3& a_rv1, 
							const glm::vec4& a_colour0, const glm::vec4& a_colour1);

	// Adds a triangle.
	static void		addTri(const glm::vec3& a_rv0, const glm::vec3& a_rv1, const glm::vec3& a_rv2, 
							const glm::vec4& a_colour);

	// Adds 3 unit-length lines (red,green,blue) representing the 3 axis of a transform, 
	// at the transform's translation. Optional scale available.
	static void		addTransform(const glm::mat4& a_transform, float a_fScale = 1.0f);
	
	// Adds an Axis-Aligned Bounding-Box with optional transform for rotation.
	static void		addBox(const glm::vec3& a_center, const glm::vec3& a_dimensions, 
							const bool& a_filled, const glm::vec4& a_fillColour = glm::vec4(1.f, 1.f, 1.f, 1.f),
							const glm::mat4& a_transform = glm::mat4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f));

	// Adds a cylinder aligned to the Y-axis with optional transform for rotation.
	static void		addCylinder(const glm::vec3& a_center, float a_radius, 
								float a_fHalfLength, unsigned int a_segments, 
								const bool& a_filled, const glm::vec4& a_fillColour = glm::vec4(1.f, 1.f, 1.f, 1.f), 
								const glm::mat4& a_transform = glm::mat4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f));

	static void		addCircle(const glm::vec3& a_center, float a_radius, unsigned int a_segments,
		const bool& a_filled, const glm::vec4& a_Colour = glm::vec4(1.f, 1.f, 1.f, 1.f),
		const glm::mat4& a_transform = glm::mat4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f));

	// Adds a Sphere at a given position, with a given number of rows, and columns, radius and a max and min long and latitude
	static void		addSphere(const glm::vec3& a_center, int a_rows, int a_columns, float a_radius, const glm::vec4& a_fillColour, 
							  const glm::mat4* a_transform = nullptr, float a_longMin = 0.f, float a_longMax = 360, 
							  float a_latMin = -90, float a_latMax = 90);

private:

	Gizmos(unsigned int a_maxLines, unsigned int a_maxTris);
	~Gizmos();

	void TestShaderStatus(const unsigned int& a_uiShaderId, unsigned int a_uiStatus, int& a_iSuccessFlag, const char* a_shaderType);
	struct GizmoVertex
	{
		glm::vec4 position;
		glm::vec4 colour;
	};

	struct GizmoLine
	{
		GizmoVertex v0;
		GizmoVertex v1;
	};

	struct GizmoTri
	{
		GizmoVertex v0;
		GizmoVertex v1;
		GizmoVertex v2;
	};

	unsigned int	m_programID;
	unsigned int	m_vertexShader;
	unsigned int	m_fragmentShader;

	// line data
	unsigned int	m_maxLines;
	unsigned int	m_lineCount;
	GizmoLine*		m_lines;

	unsigned int	m_lineVAO;
	unsigned int 	m_lineVBO;

	// triangle data
	unsigned int	m_maxTris;
	unsigned int	m_triCount;
	GizmoTri*		m_tris;

	unsigned int	m_triVAO;
	unsigned int 	m_triVBO;

	static Gizmos*	sm_singleton;
};

inline void Gizmos::draw(const glm::mat4& a_view, const glm::mat4& a_projection)
{
	draw(a_projection * a_view);
}

#endif // __GIZMOS_H_
