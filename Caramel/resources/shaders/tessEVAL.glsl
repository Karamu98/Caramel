#version 400

layout(triangles, equal_spacing, ccw) in;

in TessCtrl
{
  vec3 pos;
  vec3 normal;
  vec2 uv;
} tessCS[];

out TessEval
{
  vec3 pos;
  vec3 normal;
  vec2 uv;
} tessEval;

uniform int hasTexture;
uniform sampler2D texture_height1;
uniform float displacementScale;

void main()
{
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;
	vec3 p = gl_TessCoord.xyz;

	tessEval.pos = tessCS[0].pos * p.x + tessCS[1].pos * p.y + tessCS[2].pos * p.z;
	tessEval.normal = normalize(tessCS[0].normal * p.x + tessCS[1].normal * p.y + tessCS[2].normal * p.z);
	tessEval.uv = tessCS[0].uv * p.x + tessCS[1].uv * p.y + tessCS[2].uv * p.z;

	float disp = 0;

	if (displacementScale > 0)
	{
		disp = texture(texture_height1, tessEval.uv).r;
	}

	gl_Position =  p0 * p.x + p1 * p.y + p2 * p.z;
	// Displace using height map

}
