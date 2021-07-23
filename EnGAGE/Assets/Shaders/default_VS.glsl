#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inUv;
layout(location = 3) in float inTexID;

out vec4 FS_Color;
out vec2 FS_Uv;
out float FS_TexID;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
	FS_Color = inColor;
	FS_Uv = inUv;
	FS_TexID = inTexID;
	gl_Position = uProjection * uView * vec4(inPos, 1);
};