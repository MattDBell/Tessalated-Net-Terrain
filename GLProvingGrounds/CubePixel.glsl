#version 420

in vec4 ocolor;
in vec4 world;
in vec4 normal;
out vec4 outColor;

//Must match Lights.h
#define MAXLIGHTS 10

layout ( std140 ) uniform Lights{
	vec4	position[MAXLIGHTS];
	vec4	color[MAXLIGHTS];
	int		type[MAXLIGHTS];
};

layout(binding = 0) uniform sampler2D samp;

void main(void){
	vec4 diffuseColor = vec4(0, 0, 0, 1);
	for(int i = 0; i < 10; ++i){
		vec4 lightDir = position[i] - world;
		float intensity = clamp(dot(normal.xyz, normalize(lightDir).xyz), 0.0, 1.0);
		diffuseColor +=  color[i] *intensity;
	}

	outColor = ocolor;
	outColor = texture(samp, outColor.xy);
	outColor = outColor * diffuseColor;
}