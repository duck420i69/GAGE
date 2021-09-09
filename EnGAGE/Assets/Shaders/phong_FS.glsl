#version 460 core

#include "uniform_bindings.glsl"
#include "light.glsl"

layout(std140, binding = 2) uniform Material {
	vec3 mat_color;
	float specular_intensity;
	int specular_power;
	bool has_texture;
};


in vec3 FS_FragPosCam;
in vec3 FS_Normal;
in vec2 FS_UV;

out vec4 out_Color;

uniform sampler2D uDiffuse;

void main()
{
	vec4 texture_color = vec4(1, 1, 1, 1);

	if(has_texture) {
		texture_color = texture(uDiffuse, FS_UV);
		if(texture_color.a < 0.1) 
			discard;
	} else {
		texture_color = vec4(mat_color, 1);
	}

	const vec3 n = normalize(FS_Normal);

	vec3 vToL = light_pos - FS_FragPosCam;
	float distToL = length(vToL);
	vec3 dirToL = vToL / distToL;

	const float att = Attenuate(att_const, att_linear, att_exponent, distToL);
	const vec3 diffuse = CalDiffuse(diffuse_color, diffuse_intensity, dirToL, n ) * att;
	const vec3 specular = att * CalSpecular(n, vToL, diffuse_color, diffuse_intensity, specular_intensity, specular_power, FS_FragPosCam);



	out_Color = vec4(clamp(((diffuse + ambient_color ) * texture_color.rgb  + specular), 0.0f, 1.0f) , texture_color.a);
}