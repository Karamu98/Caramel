#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out float gSpec;

in vec2 fragUV;
in vec3 FragPos;
in vec3 fragNorm;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    // Store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // Also store the per-fragment fragNorms into the gbuffer
    gNormal = fragNorm;
    // and the diffuse per-fragment color with alpha for blending later
    gAlbedo = texture(texture_diffuse1, fragUV);
    // Store spec
    gSpec = texture(texture_specular1, fragUV).r;
}
