#version 460 core

layout(std140, binding = 1) uniform Material {
	vec4 color;
};

out vec4 out_Color;

void main() {
	out_Color = color;
};