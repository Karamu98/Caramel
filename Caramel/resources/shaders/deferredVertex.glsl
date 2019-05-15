#version 400
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec3 aTans;
layout (location = 3) in vec3 aBiTans;
layout (location = 4) in vec2 aTexCoords;

out fragment
{
	vec3 pos;
  vec3 normal;
	vec3 tangent;
	vec2 uv;
}frag;

uniform mat4 model;
uniform sampler2D texture_normal1;
uniform mat4 projectionView;
uniform bool hasNormalMap;


void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    frag.pos = worldPos.xyz;
    frag.uv = aTexCoords;

		// Optimise map checking
		if(hasNormalMap)
		{
			frag.normal = vec3(model * vec4(aNormal.xyz, 0.0)).xyz;
			frag.tangent = vec3(model * vec4(aTans.xyz, 0.0)).xyz;
		}
		else
		{
			frag.normal = aNormal.xyz;
		}




}
