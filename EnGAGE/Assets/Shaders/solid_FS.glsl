#version 460 core

layout(std140, binding = 2) uniform Material {
	vec3 color;
};

out vec4 out_Color;

void main() {
	out_Color = vec4(color, 1);
};