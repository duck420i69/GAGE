#version 460 core
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;

out vec2 FS_TexCoord;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	FS_TexCoord = inTexCoord;
	gl_Position = uProjection * uView * uModel * vec4(inPos, 0, 1);
}