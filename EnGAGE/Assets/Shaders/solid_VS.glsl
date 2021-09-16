#version 460 core

layout(location = 0) in vec3 inPos;

layout (std140, binding = 0) uniform Transform
{
	mat4 modelView;
	mat4 modelViewProj;
};

void main() {
	gl_Position = modelViewProj * vec4(inPos, 1.0);
}