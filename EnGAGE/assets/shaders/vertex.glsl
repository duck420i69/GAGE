#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUv;

out vec3 FS_Color;
out vec2 FS_Uv;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
	FS_Color = inColor;
	FS_Uv = inUv;
	gl_Position = uProjection * uView * vec4(inPos, 1);
};