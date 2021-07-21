#version 460 core

in vec3 FS_Color;

out vec4 out_Color;

void main()
{
	float noise = fract(sin(dot(FS_Color.xy ,vec2(12.9898,78.233))) * 43758.5453);
	out_Color = vec4(FS_Color * noise, 1);
}