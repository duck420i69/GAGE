#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

layout (std140, binding = 0) uniform Transform
{
	mat4 transform;
};

out vec3 FS_Color;

void main()
{
	FS_Color = inColor;
	gl_Position = transform * vec4(inPos, 1);
};