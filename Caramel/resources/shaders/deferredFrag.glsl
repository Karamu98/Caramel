#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 fragUV;
in vec3 FragPos;
in vec3 fragNorm;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment fragNorms into the gbuffer
    gNormal = fragNorm;
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(texture_diffuse1, fragUV).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(texture_specular1, fragUV).a;
}
