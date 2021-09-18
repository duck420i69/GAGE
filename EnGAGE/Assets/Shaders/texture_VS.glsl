#version 460 core
#include "uniform_bindings.glsl"

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUv;

out vec2 FS_Uv;

void main() {
	FS_Uv = inUv;
	gl_Position = modelViewProj * vec4(inPos, 1.0);
}