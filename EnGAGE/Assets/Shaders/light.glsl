
vec3 CalDiffuse(vec3 light_color, float light_intensity, vec3 dir_to_light, vec3 n) {		
	return light_color * light_intensity * max(0.0f, dot(dir_to_light, n));
}

float Attenuate(float constant, float linear, float exponent, float distance) {
	
	return 1.0f / (constant + linear * distance + exponent * (distance * distance));
}

vec3 CalSpecular(vec3 n, vec3 vec_to_light, vec3 light_color, float light_intensity, float specular_intensity, int specular_power, vec3 frag_pos_view) {
	return (light_color * light_intensity) * specular_intensity * pow( max(0.0f, dot(normalize(reflect(vec_to_light, n)), normalize(frag_pos_view))), specular_power);
}