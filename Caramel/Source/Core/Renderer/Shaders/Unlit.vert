#version 430
layout (location = 0) in vec3 v_Pos;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_UV;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ModelTransform;


void main()
{
    FragPos = vec3(u_ModelTransform * vec4(v_Pos, 1.0));
    Normal = mat3(transpose(inverse(u_ModelTransform))) * v_Normal;
    UV = v_UV;
    gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
}