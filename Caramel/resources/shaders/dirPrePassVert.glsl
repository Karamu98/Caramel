#version 400
layout (location = 0) in vec4 aPos;


uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos.xyz, 1.0);
}
