#version 400

layout (location = 0) out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float gamma;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColour = texture(scene, TexCoords).rgb;
    vec3 bloomColour = texture(bloomBlur, TexCoords).rgb;

    hdrColour += bloomColour; // additive blending

    // tone mapping
    vec3 result =  hdrColour / (hdrColour + vec3(1.0));

    result = pow(result, vec3(1.0 / gamma));
    FragColour = vec4(result, 1.0);
}
