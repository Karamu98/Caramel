#version 400

layout(vertices = 3) out;

in fragment
{
	vec3 pos;
  vec3 normal;
	vec2 UV;
}frag[];

uniform vec3 cameraPosition;

out TessCtrl
{
	vec3 pos;
	vec3 normal;
	vec2 uv;
} tessCS[];

float GetTessLevel(float a_first, float a_second);

void main()
{
	tessCS[gl_InvocationID].pos = frag[gl_InvocationID].pos;
	tessCS[gl_InvocationID].normal = frag[gl_InvocationID].normal;
	tessCS[gl_InvocationID].uv = frag[gl_InvocationID].UV;

	// Calculate tessalation levels
	float EyeVertDist0 = distance(cameraPosition, tessCS[0].pos);
	float EyeVertDist1 = distance(cameraPosition, tessCS[1].pos);
	float EyeVertDist2 = distance(cameraPosition, tessCS[2].pos);

	gl_TessLevelOuter[0] = GetTessLevel(EyeVertDist1, EyeVertDist2);
	gl_TessLevelOuter[1] = GetTessLevel(EyeVertDist2, EyeVertDist0);
	gl_TessLevelOuter[2] = GetTessLevel(EyeVertDist0, EyeVertDist1);

	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}

float GetTessLevel(float a_first, float a_second)
{
		float avgDist = (a_first + a_second) * 0.5;

		// Find branchless way
		if(avgDist <= 2.0)
		{
			return 10.0;
		}
		else if(avgDist <= 5.0)
		{
			return 5.0;
		}
		else
		{
			return 1.0;
		}

		//float upperFirst = min(avgDist, 2.0); // get lesser
		//float upperSecond = step(upperFirst, 2.0); // 0 if it's less
		//float upperEdge = upperSecond * 10; // 1 * 10 (Tess level 10)

		//float lowerFirst = min(avgDist, 5.0); // get lesser
		//float lowerSecond = step(lowerFirst, 5.0); // 0 if it's less
		//float lowerEdge = lowerSecond * 7; // 1 * 7 (Tess level 7)

		//float level = max(lowerEdge, upperEdge); // get the value that isnt 0 or the upperedge

		//return min(level, 3); // If not 10 or 7, its far so 3
}
