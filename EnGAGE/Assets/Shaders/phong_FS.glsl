#version 460 core

layout(std140, binding = 1) uniform Light {
	vec3 light_pos;
	vec3 ambient_color;
	vec3 diffuse_color;
	float diffuse_intensity;
	float att_const;
	float att_linear;
	float att_exponent;
};

layout(std140, binding = 2) uniform Material {
	float specular_intensity;
	int specular_power;
};


//const vec3 mat_color = {0.7f, 0.7f, 0.9f};
//const vec3 ambient_color = {0.05f, 0.05f, 0.05f};
//const vec3 diffuse_color = {1.0f, 1.0f, 1.0f};
//const float diffuse_intensity = 1.0f;
//const float att_const = 1.0f;
//const float att_linear = 0.045f;
//const float att_exponent = 0.0075f;


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

	out_Color = vec4(clamp(((diffuse + ambient_color ) * texture(uDiffuse, FS_UV).rgb + specular), 0.0f, 1.0f) , 1.0f);
}