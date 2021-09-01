#version 460 core

in vec2 FS_Uv;

out vec4 out_Color;

uniform sampler2D u_Diffuse;

void main()
{
	out_Color = texture(u_Diffuse, FS_Uv);
}