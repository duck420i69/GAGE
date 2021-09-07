#version 460 core

#include "uniform_bindings.glsl"

in vec3 FS_FragPosCam;
in vec3 FS_Normal;
in vec2 FS_UV;
in vec3 FS_Tangent;
in vec3 FS_Bitangent;

out vec4 out_Color;

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;


void main()
{
	vec3 n = normalize(FS_Normal);
	const mat3 tanToView = mat3(normalize(FS_Tangent), normalize(FS_Bitangent), n);

	const vec3 normal_map = texture(uNormal, FS_UV).rgb;
	n.x = normal_map.x * 2.0f - 1.0f;
	n.y = normal_map.y * 2.0f - 1.0f;
	n.z = normal_map.z;

	n =  normalize( tanToView * n);


	const vec3 vToL = light_pos - FS_FragPosCam;
	const float distToL = length(vToL);
	const vec3 dirToL = vToL / distToL;

	const float att = 1.0f / (att_const + att_linear * distToL + att_exponent * (distToL * distToL));
	const vec3 diffuse = diffuse_color * diffuse_intensity * att * max(0.0f, dot(dirToL, n));

	const vec4 specular_sample = texture(uSpecular, FS_UV);
	const vec3 specular_map_color = specular_sample.rgb;
	const float specular_map_power = pow(2.0f, specular_sample.a * 5.0f);

	const vec3 w = n * dot(vToL, n);
	const vec3 r = -normalize(w * 2.0f - vToL);
	const vec3 specular = att * (diffuse_color * diffuse_intensity) * pow( max(0.0f, dot(r, normalize(FS_FragPosCam))), specular_map_power);


	const vec3 final_color = (diffuse + ambient_color + specular_map_color * specular) * texture(uDiffuse, FS_UV).rgb;

	out_Color = vec4(clamp(final_color, 0.0f, 1.0f) , 1.0f);
}