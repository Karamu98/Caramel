#version 400
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in TessEval
{
  vec3 pos;
  vec3 normal;
  vec3 tangent;
  vec2 uv;
} frag;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform float meshShininess;
uniform bool hasNormalMap;

// I used OGL dev to learn this (http://ogldev.atspace.co.uk/www/tutorial26/tutorial26.html)
vec3 CalcBumpedNormal()
{
    vec3 Normal = vec3(normalize(frag.normal));
    vec3 Tangent = normalize(frag.tangent);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, vec3(frag.normal));
    vec3 BumpMapNormal = max(texture(texture_normal1, frag.uv).xyz, Normal);
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main()
{

    if(hasNormalMap)
    {
      gNormal = CalcBumpedNormal();
    }
    else
    {
      gNormal = frag.normal;
    }
    // Store the fragment position vector in the first gbuffer texture
    gPosition = frag.pos;
    // Also store the per-fragment fragNorms into the gbuffer
    // and the diffuse per-fragment color
    gAlbedoSpec.xyz = min(vec3(1), texture(texture_diffuse1, frag.uv).xyz);
// Store spec and height?? r = spec, b = height
    // Store spec
    gAlbedoSpec.w = max(0.0, texture(texture_specular1, frag.uv).r);

}
