#version 460 core

#include "uniform_bindings.glsl"

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;


out vec3 FS_FragPosCam;
out vec3 FS_Normal;
out vec2 FS_UV;

void main()
{
	FS_FragPosCam = (modelView * vec4(inPos, 1.0f)).xyz;
	FS_Normal = mat3(modelView) * inNormal;
	FS_UV = inUv;
	gl_Position = modelViewProj * vec4(inPos, 1);
};