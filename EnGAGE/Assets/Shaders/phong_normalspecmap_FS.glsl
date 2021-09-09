#version 460

#include "uniform_bindings.glsl"
#include "light.glsl"

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
	vec4 texture_color =  texture(uDiffuse, FS_UV);
	if(texture_color.a < 0.1) 
		discard;

	vec3 n = normalize(FS_Normal);
	if(dot(n, FS_FragPosCam) > 0.0f) {
		n = -n;
	}

	const mat3 tanToView = mat3(normalize(FS_Tangent), normalize(FS_Bitangent), n);

	const vec3 normal_map = texture(uNormal, FS_UV).rgb;
	n.x = normal_map.x * 2.0f - 1.0f;
	n.y = normal_map.y * 2.0f - 1.0f;
	n.z = normal_map.z;

	n =  normalize( tanToView * n);


	const vec3 vToL = light_pos - FS_FragPosCam;
	const float distToL = length(vToL);
	const vec3 dirToL = vToL / distToL;

	const float att = Attenuate(att_const, att_linear, att_exponent, distToL);
	const vec3 diffuse = CalDiffuse(diffuse_color, diffuse_intensity, dirToL, n ) * att;

	const vec4 specular_sample = texture(uSpecular, FS_UV);
	const vec3 specular_map_color = specular_sample.rgb;
	const int specular_map_power = int(pow(2.0f, specular_sample.a * 13.0f));

	const vec3 specular = att * CalSpecular(n, vToL, diffuse_color, diffuse_intensity, 1.8f, specular_map_power, FS_FragPosCam) * specular_map_color;

	
	const vec3 final_color = (diffuse + ambient_color) * texture_color.rgb + specular;

	out_Color = vec4(clamp(final_color, 0.0f, 1.0f) , texture_color.a);
}