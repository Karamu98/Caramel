#include "Gizmos.h"
#include "Error.h"
#include <sstream>
#include "gl_core_4_4.h"
#include <glm/ext.hpp>

#define GLSL(src) "#version 330 core\n" #src

Gizmos* Gizmos::sm_singleton = nullptr;

Gizmos::Gizmos(unsigned int a_maxLines, unsigned int a_maxTris)
	: m_maxLines(a_maxLines),
	m_maxTris(a_maxTris),
	m_lineCount(0),
	m_triCount(0),
	m_lines(new GizmoLine[a_maxLines]),
	m_tris(new GizmoTri[a_maxTris])
{
	int success = GL_FALSE;
	//\==============================================================================================
	//\ Create our Vertex Shader from a char array
	const char* vsSource = GLSL(
					 in vec4 Position; 
					 in vec4 Colour; 
					 out vec4 vColour; 
					 uniform mat4 ProjectionView; 
					 void main() { vColour = Colour; gl_Position = ProjectionView * Position; });

	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);	
	glShaderSource(m_vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(m_vertexShader);
	//Test our shader for succesful compiliation
	TestShaderStatus(m_vertexShader, GL_COMPILE_STATUS, success, "Vertex");

	const char* fsSource = GLSL(
					 in vec4 vColour; 
					 out vec4 FragColor; 
					 void main()	{ FragColor = vColour; });

	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(m_fragmentShader);
	TestShaderStatus(m_fragmentShader, GL_COMPILE_STATUS, success, "Fragment");
	
	
	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertexShader);
	glAttachShader(m_programID, m_fragmentShader);
	glBindAttribLocation(m_programID, 0, "Position");
	glBindAttribLocation(m_programID, 1, "Colour");
	glLinkProgram(m_programID);
	TestShaderStatus(m_fragmentShader, GL_LINK_STATUS, success, nullptr);
	
	// create VBOs
	glGenBuffers( 1, &m_lineVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxLines * sizeof(GizmoLine), m_lines, GL_DYNAMIC_DRAW);


	glGenBuffers( 1, &m_triVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_triVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxTris * sizeof(GizmoTri), m_tris, GL_DYNAMIC_DRAW);


	glGenVertexArrays(1, &m_lineVAO);
	glBindVertexArray(m_lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(GizmoVertex), ((char*)0) + 16);

	glGenVertexArrays(1, &m_triVAO);
	glBindVertexArray(m_triVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_triVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(GizmoVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Gizmos::~Gizmos()
{
	delete[] m_lines;
	delete[] m_tris;
	glDeleteBuffers( 1, &m_lineVBO );
	glDeleteBuffers( 1, &m_triVBO );
	glDeleteVertexArrays( 1, &m_lineVAO );
	glDeleteVertexArrays( 1, &m_triVAO );
	glDeleteProgram(m_programID);
	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);
}

void Gizmos::create(unsigned int a_maxLines /* = 16384 */, unsigned int a_maxTris /* = 16384 */)
{
	if (sm_singleton == nullptr)
		sm_singleton = new Gizmos(a_maxLines,a_maxTris);
}

void Gizmos::destroy()
{
	delete sm_singleton;
	sm_singleton = nullptr;
}

void Gizmos::clear()
{
	sm_singleton->m_lineCount = 0;
	sm_singleton->m_triCount = 0;
}

// Adds 3 unit-length lines (red,green,blue) representing the 3 axis of a transform, 
// at the transform's translation. Optional scale available.
void Gizmos::addTransform(const glm::mat4& a_transform, float a_fScale /* = 1.0f */)
{
	glm::vec4 vXAxis = a_transform[3] + a_transform[0] * a_fScale;
	glm::vec4 vYAxis = a_transform[3] + a_transform[1] * a_fScale;
	glm::vec4 vZAxis = a_transform[3] + a_transform[2] * a_fScale;

	glm::vec4 vRed(1,0,0,1);
	glm::vec4 vGreen(0,1,0,1);
	glm::vec4 vBlue(0,0,1,1);

	addLine(a_transform[3].xyz, vXAxis.xyz, vRed, vRed);
	addLine(a_transform[3].xyz, vYAxis.xyz, vGreen, vGreen);
	addLine(a_transform[3].xyz, vZAxis.xyz, vBlue, vBlue);
}


void Gizmos::addBox(const glm::vec3& a_center, const glm::vec3& a_vDimentions,
	const bool& a_filled,
	const glm::vec4& a_fillColour, 
	const glm::mat4& a_transform)
{
	glm::vec3 vX(a_vDimentions.x * 0.5f, 0.f, 0.f);
	glm::vec3 vY(0.f, a_vDimentions.y * 0.5f, 0.f);
	glm::vec3 vZ(0.f, 0.f, a_vDimentions.z * 0.5f);

	// Apply transform
	vX = (a_transform * glm::vec4(vX, 0.f)).xyz;
	vY = (a_transform * glm::vec4(vY, 0.f)).xyz;
	vZ = (a_transform * glm::vec4(vZ, 0.f)).xyz;

	glm::vec3 vVerts[8];

	// Bottom Vertices
	vVerts[0] = a_center - vX - vZ - vY;
	vVerts[1] = a_center - vX + vZ - vY;
	vVerts[2] = a_center + vX + vZ - vY;
	vVerts[3] = a_center + vX - vZ - vY;

	// Up Vertices
	vVerts[4] = a_center - vX - vZ + vY;
	vVerts[5] = a_center - vX + vZ + vY;
	vVerts[6] = a_center + vX + vZ + vY;
	vVerts[7] = a_center + vX - vZ + vY;

	glm::vec4 vWhite = glm::vec4(1.f, 1.f, 1.f, 1.f);

	// Connect the dots
	// Base
	addLine(vVerts[0], vVerts[1], vWhite);
	addLine(vVerts[1], vVerts[2], vWhite);
	addLine(vVerts[2], vVerts[3], vWhite);
	addLine(vVerts[3], vVerts[0], vWhite);
	// Top
	addLine(vVerts[4], vVerts[5], vWhite);
	addLine(vVerts[5], vVerts[6], vWhite);
	addLine(vVerts[6], vVerts[7], vWhite);
	addLine(vVerts[7], vVerts[4], vWhite);
	// Vertical
	addLine(vVerts[0], vVerts[4], vWhite);
	addLine(vVerts[1], vVerts[5], vWhite);
	addLine(vVerts[2], vVerts[6], vWhite);
	addLine(vVerts[3], vVerts[7], vWhite);

	if (a_filled)
	{
		// Base face
		addTri(vVerts[2], vVerts[1], vVerts[0], a_fillColour);
		addTri(vVerts[3], vVerts[2], vVerts[0], a_fillColour);
		// Top face
		addTri(vVerts[5], vVerts[6], vVerts[4], a_fillColour);
		addTri(vVerts[6], vVerts[7], vVerts[4], a_fillColour);
		// Front face
		addTri(vVerts[4], vVerts[3], vVerts[0], a_fillColour);
		addTri(vVerts[7], vVerts[3], vVerts[4], a_fillColour);
		// Back face
		addTri(vVerts[1], vVerts[2], vVerts[5], a_fillColour);
		addTri(vVerts[2], vVerts[6], vVerts[5], a_fillColour);
		// Left face
		addTri(vVerts[0], vVerts[1], vVerts[4], a_fillColour);
		addTri(vVerts[1], vVerts[5], vVerts[4], a_fillColour);
		// Right face
		addTri(vVerts[2], vVerts[3], vVerts[7], a_fillColour);
		addTri(vVerts[6], vVerts[2], vVerts[7], a_fillColour);
	}
}

void Gizmos::addCircle(const glm::vec3& a_center, float a_radius, unsigned int a_segments,
	const bool& a_filled, const glm::vec4& a_Colour,
	const glm::mat4& a_transform)
{
	// Calculate the innter angle for each segment
	float fAngle = (2.f * glm::pi<float>()) / a_segments;
	glm::vec4 vEdge(0, 0, a_radius, 0);

	for (int i = 0; i < a_segments; ++i)
	{
		glm::vec4 vEdge2(sinf((i + 1) * fAngle) * a_radius, 0, cosf((i + 1) * fAngle) * a_radius, 0);
		vEdge = a_transform * vEdge;
		vEdge2 = a_transform * vEdge2;

		addLine(a_center + vEdge.xyz, a_center + vEdge2.xyz, a_Colour);

		if (a_filled)
		{
			addTri(a_center, a_center + vEdge.xyz, a_center + vEdge2.xyz, a_Colour);
			addTri(a_center, a_center + vEdge2.xyz, a_center + vEdge.xyz, a_Colour);
		}
		vEdge = vEdge2;
	}
}

void Gizmos::addCylinder(const glm::vec3& a_center, float a_radius,
	float a_fHalfLength, unsigned int a_segments,
	const bool& a_filled, const glm::vec4& a_fillColour,
	const glm::mat4& a_transform)
{

}

void Gizmos::addLine(const glm::vec3& a_rv0,  const glm::vec3& a_rv1, const glm::vec4& a_colour)
{
	addLine(a_rv0,a_rv1,a_colour,a_colour);
}

void Gizmos::addLine(const glm::vec3& a_rv0, const glm::vec3& a_rv1, const glm::vec4& a_colour0, const glm::vec4& a_colour1)
{
	if (sm_singleton != nullptr &&
		sm_singleton->m_lineCount < sm_singleton->m_maxLines)
	{
		sm_singleton->m_lines[ sm_singleton->m_lineCount ].v0.position = glm::vec4(a_rv0,1);
		sm_singleton->m_lines[ sm_singleton->m_lineCount ].v0.colour = a_colour0;
		sm_singleton->m_lines[ sm_singleton->m_lineCount ].v1.position = glm::vec4(a_rv1,1);
		sm_singleton->m_lines[ sm_singleton->m_lineCount ].v1.colour = a_colour1;

		sm_singleton->m_lineCount++;
	}
}

void Gizmos::addTri(const glm::vec3& a_rv0, const glm::vec3& a_rv1, const glm::vec3& a_rv2, const glm::vec4& a_colour)
{
	if (sm_singleton != nullptr)
	{
		if (sm_singleton->m_triCount < sm_singleton->m_maxTris)
		{
			sm_singleton->m_tris[sm_singleton->m_triCount].v0.position = glm::vec4(a_rv0, 1);
			sm_singleton->m_tris[sm_singleton->m_triCount].v1.position = glm::vec4(a_rv1, 1);
			sm_singleton->m_tris[sm_singleton->m_triCount].v2.position = glm::vec4(a_rv2, 1);
			sm_singleton->m_tris[sm_singleton->m_triCount].v0.colour = a_colour;
			sm_singleton->m_tris[sm_singleton->m_triCount].v1.colour = a_colour;
			sm_singleton->m_tris[sm_singleton->m_triCount].v2.colour = a_colour;

			sm_singleton->m_triCount++;
		}
	}
}

void Gizmos::draw(const glm::mat4& a_projectionView)
{
	if ( sm_singleton != nullptr &&
		(sm_singleton->m_lineCount > 0 || sm_singleton->m_triCount > 0 ))
	{
		glUseProgram(sm_singleton->m_programID);
		
		unsigned int projectionViewUniform = glGetUniformLocation(sm_singleton->m_programID,"ProjectionView");
		glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(a_projectionView));

		if (sm_singleton->m_lineCount > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, sm_singleton->m_lineVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sm_singleton->m_lineCount * sizeof(GizmoLine), sm_singleton->m_lines);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glBindVertexArray(sm_singleton->m_lineVAO);
			glDrawArrays(GL_LINES, 0, sm_singleton->m_lineCount * 2);
		}

		if (sm_singleton->m_triCount > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, sm_singleton->m_triVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sm_singleton->m_triCount * sizeof(GizmoTri), sm_singleton->m_tris);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(sm_singleton->m_triVAO);
			glDrawArrays(GL_TRIANGLES, 0, sm_singleton->m_triCount * 3);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
		
	}
}

void Gizmos::TestShaderStatus(const unsigned int& a_uiShaderId, unsigned int a_uiStatus, int& a_iSuccessFlag, const char* a_shaderType)
{
	if (a_uiStatus == GL_LINK_STATUS)
	{
		glGetProgramiv(a_uiShaderId, a_uiStatus, &a_iSuccessFlag);
	}
	else
	{
		glGetShaderiv(a_uiShaderId, a_uiStatus, &a_iSuccessFlag);
	}
	
	if (a_iSuccessFlag == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(a_uiShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *log = (GLchar *)malloc(infoLogLength);
		glGetProgramInfoLog(a_uiShaderId, infoLogLength, &infoLogLength, log);
		std::ostringstream ssError;
		if (a_uiStatus == GL_LINK_STATUS)
		{
			ssError << "Error: Failed to  link shader!\n" << log;
		}
		else
		{
			ssError << "Error: Failed to compile " << a_shaderType << "shader!\n" << log;
		}
		Error::RuntimeError(ssError.str());
		free(log);
	}
}