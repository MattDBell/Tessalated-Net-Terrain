#version 150



in vec4 ocolor;
in vec3 lightDir;
in vec2 coefficients;
in vec3 camDir;
out vec4 outColor;

void main(void){
	vec3 tanX = vec3(1, 0.25 * sin(coefficients.y) * cos(coefficients.x), 0);
	vec3 tanZ = vec3(0, 0.25 * sin(coefficients.x) * cos(coefficients.y), 1);
	
	vec3 onorm = cross(tanZ, tanX);
	onorm = normalize(onorm);
	
	//diffuse
	float lambert = dot(onorm, lightDir);
	vec4 diffuse = vec4(0.2, 0.2, 0.2, 0) * lambert;
	
	//specular
	vec4 specCompMult = vec4(0.2, 0.2, 0.2, 0);
	vec3 ref = reflect(-lightDir, onorm);
	vec4 specular = specCompMult * pow(max(dot(ref, camDir), 0.0), 3);
	
	float fresnel = 0.3 + (1-0.2) * pow((1 - cos( acos( dot(lightDir, camDir * vec3 (1, -1, 1)))/2)), 5);
	
	
	outColor = ocolor + diffuse + specular + vec4(1, 1, 1, 0) * fresnel;
	
}