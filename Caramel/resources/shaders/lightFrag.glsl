#version 400

layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BloomColour;

uniform vec3 lightColour;

void main()
{
    FragColour = vec4(lightColour , 1.0);
    BloomColour = vec4(lightColour * 5.0 , 1.0);
}
