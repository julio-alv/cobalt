#version 410 core

layout(location = 0) out vec4 fragColor;

// uniform vec4 u_Color;

in vec4 vertexColor;

void main() { fragColor = vertexColor; }