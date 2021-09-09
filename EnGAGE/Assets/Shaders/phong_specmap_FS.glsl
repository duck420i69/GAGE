#version 460 core

#include "uniform_bindings.glsl"
#include "light.glsl"

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


void main()
{
	vec4 texture_color =  texture(uDiffuse, FS_UV);
	//if(texture_color.a < 0.1) 
			//discard;

	const vec3 n = normalize(FS_Normal);
	const vec3 vToL = light_pos - FS_FragPosCam;
	const float distToL = length(vToL);
	const vec3 dirToL = vToL / distToL;

	const float att = Attenuate(att_const, att_linear, att_exponent, distToL);
	const vec3 diffuse = CalDiffuse(diffuse_color, diffuse_intensity, dirToL, n ) * att;

	const vec4 specular_sample = texture(uSpecular, FS_UV);
	const vec3 specular_map_color = specular_sample.rgb;
	const int specular_map_power = int(pow(2.0f, specular_sample.a * 13.0f));

	const vec3 specular = att * CalSpecular(n, vToL, diffuse_color, diffuse_intensity, 1.8f, specular_map_power, FS_FragPosCam) * specular_map_color;

	const vec3 final_color = (diffuse + ambient_color) * texture_color.rgb  + specular;

	out_Color = vec4(clamp(final_color, 0.0f, 1.0f) , texture_color.a);
}