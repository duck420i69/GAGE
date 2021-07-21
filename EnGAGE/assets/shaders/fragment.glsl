#version 460 core

in vec3 FS_Color;

out vec4 out_Color;

void main()
{
	out_Color = vec4(FS_Color, 1);
}