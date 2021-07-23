#version 460 core

const int MAX_TEXTURES = 8;

in vec4 FS_Color;
in vec2 FS_Uv;
in float FS_TexID;

out vec4 out_Color;

uniform sampler2D uTextures[MAX_TEXTURES];

void main()
{
	if(FS_TexID > 0)
	{
		int id = int(FS_TexID);
		out_Color = FS_Color * texture(uTextures[id], FS_Uv);
	} else
	{
		out_Color = FS_Color;
	}
}