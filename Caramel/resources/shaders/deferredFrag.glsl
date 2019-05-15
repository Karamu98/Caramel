#version 400
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out float gSpec;

in TessEval
{
  vec3 pos;
  vec3 normal;
  vec2 uv;
} frag;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float meshSpecular;

void main()
{
    // Store the fragment position vector in the first gbuffer texture
    gPosition = frag.pos;
    // Also store the per-fragment fragNorms into the gbuffer
    gNormal = frag.normal;
    // and the diffuse per-fragment color
    gAlbedo = max(vec3(0), texture(texture_diffuse1, frag.uv).xyz);
// Store spec and height?? r = spec, b = height
    // Store spec
    gSpec = max(0.0, texture(texture_specular1, frag.uv).r);

}
