#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

out vec3 FS_Color;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
	FS_Color = inColor;
	gl_Position = uProjection * uView * vec4(inPos, 1);
};