#version 460 core
#include "uniform_bindings.glsl"

in vec2 FS_Uv;

out vec4 out_Color;

uniform sampler2D uDiffuse;

void main() {
	out_Color = texture(uDiffuse, FS_Uv);
}

