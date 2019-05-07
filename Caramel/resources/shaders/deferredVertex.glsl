#version 400
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 4) in vec2 aTexCoords;

out fragment
{
	vec3 pos;
	vec3 normal;
	vec2 UV;
}frag;

uniform mat4 model;
uniform mat4 viewMatrix;
uniform mat4 projectionView;

void main()
{
    vec4 worldPos = model * aPos;
    frag.pos = worldPos.xyz;
    frag.UV = aTexCoords;

    frag.normal = aNormal.xyz;

    gl_Position = projectionView * worldPos;
}
