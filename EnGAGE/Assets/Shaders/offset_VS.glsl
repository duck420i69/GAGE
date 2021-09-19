#version 460 core


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout (std140, binding = 0) uniform Transform
{
	mat4 modelView;
	mat4 modelViewProj;
};

layout(std140, binding = 3) uniform Reserved{
	float offset;	
};


void main() {
	gl_Position = modelViewProj * vec4(inPos + inNormal * offset, 1.0);
}