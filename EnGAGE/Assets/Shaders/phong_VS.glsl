#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout (std140, binding = 0) uniform Transform
{
	mat4 modelView;
	mat4 modelViewProj;
};

out vec3 FS_FragPosCam;
out vec3 FS_Normal;

void main()
{
	FS_FragPosCam = (modelView * vec4(inPos, 1.0f)).xyz;
	FS_Normal = mat3(modelView) * inNormal;
	gl_Position = modelViewProj * vec4(inPos, 1);
};