#type vertex
#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;

uniform mat4 camProjView;
uniform mat4 objMatrix;

out Vertex
{
	vec4 pos;
	vec3 normal;
}vert;

void main()
{
	vert.pos = objMatrix * vec4(aPos, 1.0);
	vert.normal = (objMatrix * vec4(aNorm, 1.0)).xyz;
	gl_Position = camProjView * vert.pos;
}

#type fragment
#version 330

in Vertex
{
	vec4 pos;
	vec3 normal;
}vert;

layout (location = 0) out vec3 gTex;

uniform vec3 previewColour;

void main()
{
	vec3 lightPos = vec3(1, 1, 1);

	// Ambient
	vec3 ambient = previewColour * 0.05f;

	// Diffuse
	vec3 lightDir = normalize(lightPos - vert.pos.xyz);
	vec3 normal = normalize(vert.normal);
	float diff = max(dot(lightDir, normal), 0.0);

	gTex = previewColour;
}