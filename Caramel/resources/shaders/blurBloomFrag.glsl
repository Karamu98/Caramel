#version 400

layout (location = 0) out vec4 FragColour;

uniform vec2 resolution;
uniform sampler2D gBloomPass;
uniform sampler2D gFinalDraw;
uniform vec2 direction;
uniform float hdrGamma;

// Function from https://github.com/Jam3/glsl-fast-gaussian-blur/blob/master/9.glsl
vec4 blur(sampler2D image, vec2 uv, vec2 resolution, vec2 direction)
{
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.3333333333333333) * direction;
  color += texture2D(image, uv) * 0.29411764705882354;
  color += texture2D(image, uv + (off1 / resolution)) * 0.35294117647058826;
  color += texture2D(image, uv - (off1 / resolution)) * 0.35294117647058826;
  return color;
}

in vec2 TexCoords;

void main()
{
    vec3 bloomColour = blur(gBloomPass, TexCoords, resolution, direction).xyz;
    vec3 finalColour = texture2D(gFinalDraw, TexCoords).xyz;

    finalColour += bloomColour;

    /// Reinhard tone mapping
    vec3 result =  finalColour / (finalColour + vec3(1.0));

    // Gamma correction
    result = pow(result, vec3(1.0 / hdrGamma));


    FragColour = vec4(result , 1.0);
}
