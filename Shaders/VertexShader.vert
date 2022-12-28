#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform float horizontalOffset;

void main()
{
   gl_Position = vec4(aPos.x + horizontalOffset, aPos.y, aPos.z, 1.0);
   vertexColor = aColor;
}