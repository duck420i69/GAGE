#version 460 core

#include "uniform_bindings.glsl"

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
	const vec3 n = normalize(FS_Normal);
	const vec3 vToL = light_pos - FS_FragPosCam;
	const float distToL = length(vToL);
	const vec3 dirToL = vToL / distToL;

	const float att = 1.0f / (att_const + att_linear * distToL + att_exponent * (distToL * distToL));
	const vec3 diffuse = diffuse_color * diffuse_intensity * att * max(0.0f, dot(dirToL, n));

	const vec3 w = n * dot(vToL, n);
	const vec3 r = -normalize(w * 2.0f - vToL);
	const vec3 specular = att * (diffuse_color * diffuse_intensity) * specular_intensity * pow( max(0.0f, dot(r, normalize(FS_FragPosCam))), specular_power);

	vec3 texture_color = vec3(1, 1, 1);

	if(has_texture) {
		texture_color = texture(uDiffuse, FS_UV).rgb;
	} else {
		texture_color = mat_color;
	}

	out_Color = vec4(clamp(((diffuse + ambient_color ) * texture_color  + specular), 0.0f, 1.0f) , 1.0f);
}