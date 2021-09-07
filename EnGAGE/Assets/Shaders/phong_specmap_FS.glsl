#version 460 core

#include "uniform_bindings.glsl"

layout(std140, binding = 2) uniform Material {
	float specular_intensity;
	int specular_power;
};

in vec3 FS_FragPosCam;
in vec3 FS_Normal;
in vec2 FS_UV;

out vec4 out_Color;

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;

const int specular_power_factor = 13;

void main()
{
	const vec3 n = normalize(FS_Normal);
	const vec3 vToL = light_pos - FS_FragPosCam;
	const float distToL = length(vToL);
	const vec3 dirToL = vToL / distToL;

	const float att = 1.0f / (att_const + att_linear * distToL + att_exponent * (distToL * distToL));
	const vec3 diffuse = diffuse_color * diffuse_intensity * att * max(0.0f, dot(dirToL, n));

	const vec4 specular_sample = texture(uSpecular, FS_UV);
	const vec3 specular_map_color = specular_sample.rgb;
	const float specular_map_power = pow(2, specular_sample.a * specular_power_factor);

	const vec3 w = n * dot(vToL, n);
	const vec3 r = -normalize(w * 2.0f - vToL);
	const vec3 specular = att * (diffuse_color * diffuse_intensity) * pow( max(0.0f, dot(r, normalize(FS_FragPosCam))), specular_map_power);

	out_Color = vec4(clamp((diffuse + ambient_color + specular *  specular_map_color * specular_intensity) * texture(uDiffuse, FS_UV).rgb, 0.0f, 1.0f) , 1.0f);
}