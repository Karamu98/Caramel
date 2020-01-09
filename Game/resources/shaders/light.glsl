#type vertex
#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

uniform mat4 camProjView;
uniform mat4 objMatrix;

out Vertex
{
    vec4 pos;
}vert;


void main()
{
    vert.pos = camProjView * objMatrix * vec4(aPos, 1.0);
    gl_Position = vert.pos;
}

#type fragment
#version 330

uniform vec3 gLightColour;
out vec4 FragColour;

in Vertex
{
    vec4 pos;
}vert;



void main()
{
    FragColour = vec4(gLightColour, 1.0);
}