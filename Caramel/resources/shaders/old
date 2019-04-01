#version 330


layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Normal;
layout (location = 2) in vec4 Tans;
layout (location = 3) in vec4 BiTans;
layout (location = 4) in vec2 UV;

out Vertex
{
  vec4 vPosition;
  vec4 vNormal;
  vec4 vTans;
  vec4 vBiTans;
  vec2 vUV;
  vec3 HalfDir;
  vec3 LightDir;
}vertex;

uniform float Time;
uniform mat4 ProjectionView;
uniform mat4 Model;
uniform mat4 NormalMatrix;
uniform mat4 ViewMatrix;

uniform vec4 cameraPosition;
uniform vec4 lightDirection;

void main()
{
  vec4 vNormal = Normal;
  vec2 vUV = UV;
  vec4 vTans = Tans;
  vec4 vBiTans = BiTans;

  vertex.vPosition = Model * Position;
  vertex.vNormal = Normal;
  vertex.vTans = Tans;
  vertex.vBiTans = BiTans;
  vertex.vUV = UV;

  vec4 directionOfLight = lightDirection;

  directionOfLight.x = sin(Time * 2.5);

  vertex.LightDir = (Model * -directionOfLight).xyz;

  vec3 eyeDir = (ViewMatrix * normalize( cameraPosition - vertex.vPosition)).xyz;
  vertex.HalfDir = normalize(eyeDir + vertex.LightDir);

  gl_Position = ProjectionView * Model * Position;
}
