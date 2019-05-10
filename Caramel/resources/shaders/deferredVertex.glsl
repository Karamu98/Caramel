#version 400
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 4) in vec2 aTexCoords;

out fragment
{
	vec3 pos;
	vec3 normal;
	vec2 UV;
}frag;

uniform mat4 model;
uniform mat4 projectionView;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    frag.pos = worldPos.xyz;
    frag.UV = aTexCoords;

    frag.normal = aNormal.xyz;

    gl_Position = projectionView * worldPos;
}
