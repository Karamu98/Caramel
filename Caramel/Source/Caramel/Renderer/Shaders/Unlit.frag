#version 330 core
out vec4 FragColour;

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;


// Fill from assimp
uniform vec3 u_DiffuseColour;
uniform sampler2D u_DiffuseMap0;

void main()
{
    // Texture samples
    vec3 diffuseSample = texture(u_DiffuseMap0, UV).rgb;
    
    vec3 result = u_DiffuseColour * diffuseSample;
    FragColour = vec4(result, 1.0);
} 