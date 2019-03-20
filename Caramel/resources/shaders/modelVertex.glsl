#version 150


in vec4 Position;
in vec4 Normal;
in vec2 UV;
in vec4 Tans;
in vec4 BiTans;

out Vertex
{
  vec4 vPosition;
  vec4 vNormal;
  vec2 vUV;
  vec4 vTans;
  vec4 vBiTans;
  vec3 HalfDir;
  vec3 LightDir;
}vertex;

uniform mat4 ProjectionView;
uniform mat4 Model;
uniform mat4 NormalMatrix;
uniform mat4 ViewMatrix;

uniform vec4 cameraPosition;
uniform vec4 lightDirection;

void main()
{
  vNormal = Normal;
  vUV = UV;
  vTans = Tans;
  vBiTans = BiTans;

  vertex.vPosition = Model * Position;
  vertex.vNormal = ViewMatrix * NormalMatrix * Normal;
  vertex.vUV = UV;
  vertex.vTans = Tans;
  vertex.vBiTans = BiTans;

  vertex.LightDir = (ViewMatrix * -lightDirection).xyz;

  vec3 eyeDir = (ViewMatrix * normalize( cameraPosition - vertex.vPosition)).xyz;
  vertex.HalfDir = normalize(eyeDir + vertex.LightDir);

  gl_Position = ProjectionView * Model * Position;
}
