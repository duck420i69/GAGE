#version 460 core

in vec3 FS_Color;
in vec2 FS_Uv;

out vec4 out_Color;

uniform sampler2D uTexSampler;

void main()
{
	out_Color = vec4(FS_Color, 1);
	//out_Color = texture(uTexSampler, FS_Uv);
}