#version 460 core

layout(location = 0) in vec3 inPos;

uniform mat4 uProj, uView, uModel;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(inPos, 1);
}