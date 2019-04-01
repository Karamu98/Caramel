#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 4) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;

    gl_Position = aPos;
}
