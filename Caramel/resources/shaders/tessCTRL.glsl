#version 400

layout(vertices = 3) out;

in fragment
{
	vec3 pos;
  vec3 normal;
	vec2 UV;
	float displacement;

}frag[];

uniform int innerEdge;
uniform int outerEdge;

out TessCtrl
{
	vec3 pos;
	vec3 normal;
	vec2 uv;
	float displacement;
} tessCS[];

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tessCS[gl_InvocationID].pos = frag[gl_InvocationID].pos;
	tessCS[gl_InvocationID].normal = frag[gl_InvocationID].normal;
	tessCS[gl_InvocationID].uv = frag[gl_InvocationID].UV;
	tessCS[gl_InvocationID].displacement = frag[gl_InvocationID].displacement;

	gl_TessLevelOuter[0] = outerEdge;
	gl_TessLevelOuter[1] = outerEdge;
	gl_TessLevelOuter[2] = outerEdge;

	gl_TessLevelInner[0] = innerEdge;
}
