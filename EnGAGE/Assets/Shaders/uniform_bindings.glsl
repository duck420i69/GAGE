
layout (std140, binding = 0) uniform Transform
{
	mat4 modelView;
	mat4 modelViewProj;
};

layout(std140, binding = 1) uniform Light {
	vec3 light_pos;
	vec3 light_ambient;
	vec3 light_diffuse;
	float light_intensity;
	float att_const;
	float att_linear;
	float att_exponent;
};

layout(std140, binding = 2) uniform Material {
	vec3 mat_color;
	vec3 mat_specular_color;
	int specular_power;
	bool has_diffuse;
	bool has_specular;
	bool has_normal;
};

