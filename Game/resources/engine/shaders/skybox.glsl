#type vertex
#version 330
layout (location = 0) in vec3 aPos;

uniform mat4 proj;
uniform mat4 view;

out vec3 UV;

void main()
{
    UV = aPos;
    vec4 pos = proj * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#type fragment
#version 330

out vec4 FragColour;

in vec3 UV;
uniform samplerCube skybox;

void main()
{
    FragColour = texture(skybox, UV);
}