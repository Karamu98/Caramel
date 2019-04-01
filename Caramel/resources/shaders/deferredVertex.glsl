#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 4) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 fragUV;
out vec3 fragNorm;

uniform mat4 model;
uniform mat4 viewMatrix;
uniform mat4 projectionView;

void main()
{
    vec4 worldPos = model * aPos;
    FragPos = worldPos.xyz;
    fragUV = aTexCoords;

    fragNorm = aNormal.xyz;

    gl_Position = projectionView * worldPos;
}
