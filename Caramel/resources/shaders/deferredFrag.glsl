#version 400
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
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
    // and the diffuse per-fragment color with alpha for blending later
    vec4 albedo = texture(texture_diffuse1, frag.uv);
    if(length(albedo) > 0)
    {
      gAlbedo = albedo;
    }
    else
    {
      gAlbedo = vec4(1, 1, 1, 1);
    }

// Store spec and height?? r = spec, b = height
    // Store spec
    float spec = texture(texture_specular1, frag.uv).r;

    if(spec > 0.0f)
    {
      gSpec = spec;
    }
    else
    {
      gSpec = meshSpecular;
    }
}
