#version 460 core

in vec2 FS_TexCoord;

out vec4 out_Color;

uniform sampler2D uTexture;
uniform float uAlpha;

void main() {
	out_Color = texture(uTexture, FS_TexCoord);
	out_Color.a -= uAlpha;
}