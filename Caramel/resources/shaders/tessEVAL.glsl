#version 400

layout(triangles, equal_spacing, ccw) in;

in TessCtrl
{
  vec3 pos;
	vec3 normal;
	vec3 tangent;
	vec2 uv;
} tessCS[];

out TessEval
{
  vec3 pos;
  vec3 normal;
  vec3 tangent;
  vec2 uv;
} tessEval;

uniform sampler2D texture_normal1;
uniform mat4 projectionView;
uniform float displacementScale;

vec2 Interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 Interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main()
{
  tessEval.pos = Interpolate3D(tessCS[0].pos, tessCS[1].pos, tessCS[2].pos);
  tessEval.normal = Interpolate3D(tessCS[0].normal, tessCS[1].normal, tessCS[2].normal);
  tessEval.normal = normalize(tessEval.normal);
  tessEval.tangent = Interpolate3D(tessCS[0].tangent, tessCS[1].tangent, tessCS[2].tangent);
  tessEval.tangent = normalize(tessEval.tangent);
  tessEval.uv = Interpolate2D(tessCS[0].uv, tessCS[1].uv, tessCS[2].uv);

  // Displace using normal if one is avalible
  float disp = max(0.0, texture(texture_normal1, tessEval.uv).x);
  tessEval.pos += tessEval.normal * disp * displacementScale;

	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;
	vec3 p = gl_TessCoord.xyz;

  gl_Position = projectionView * vec4(tessEval.pos, 1.0);
}
