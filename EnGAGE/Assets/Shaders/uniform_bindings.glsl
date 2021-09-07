
layout (std140, binding = 0) uniform Transform
{
	mat4 modelView;
	mat4 modelViewProj;
};

layout(std140, binding = 1) uniform Light {
	vec3 light_pos;
	vec3 ambient_color;
	vec3 diffuse_color;
	float diffuse_intensity;
	float att_const;
	float att_linear;
	float att_exponent;
};
