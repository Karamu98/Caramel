#version 400
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 4) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 fragUV;
out vec3 fragNorm;

uniform mat4 model;
uniform mat4 viewMatrix;
uniform mat4 projectionView;
uniform float Time;

void main()
{
  float diffY = ((model * aPos).x + ((model * aPos).z * 0.2));
  float yOffset = sin((Time * 2.5) + diffY);

  vec4 worldPos = model * (aPos + vec4(0, yOffset, 0, 0));
  FragPos = worldPos.xyz;
  fragUV = aTexCoords;

  fragNorm = aNormal.xyz;

  gl_Position = projectionView * worldPos;
}
