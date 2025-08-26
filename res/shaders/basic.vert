#version 410 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
    TexCoord = texCoord;
}