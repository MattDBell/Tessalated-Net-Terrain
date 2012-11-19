#version 420

in PixelData{
	vec3 normal;
	vec4 worldPos;
	vec4 shapeColor;
};

//Must match Lights.h
#define MAXLIGHTS 10

layout ( std140 ) uniform Lights{
	vec4	position[MAXLIGHTS];
	vec4	color[MAXLIGHTS];
	int		type[MAXLIGHTS];
};

out vec4 outColor;
void main(void){
	vec4 diffuseColor = vec4(0, 0, 0, 1);
	for(int i = 0; i < 10; ++i){
		vec4 lightDir = position[i] - worldPos;
		float intensity = clamp(dot(normal, normalize(lightDir).xyz), 0.0, 1.0);
		diffuseColor +=  color[i] *intensity;
	}

	outColor = shapeColor;
	outColor = outColor * diffuseColor;
}