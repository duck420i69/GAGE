#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUv;

layout (std140, binding = 0) uniform Transform
{
	mat4 transform;
};

out vec2 FS_Uv;

void main()
{
	FS_Uv = inUv;
	gl_Position = transform * vec4(inPos, 1);
};