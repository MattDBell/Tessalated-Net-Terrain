#version 420

layout( binding = 2 ) uniform sampler2D tileTex;
//layout( binding = 3 ) uniform sampler2D tileNorm;

in PixelData{
	vec3 normal;
	noperspective vec4 objectPos;
	vec4 worldPos;
};

//Must match Lights.h
#define MAXLIGHTS 10

uniform int TexMap;
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
	float normalSum = abs(normal.x) + abs(normal.y) + abs(normal.z);
	vec3 planarTextureCoeffictions = vec3(  abs(normal.x)/normalSum, abs(normal.y)/normalSum, abs(normal.z)/normalSum );
	//vec4 xColor = vec4(1, 0, 0, 1);
	//vec4 yColor = vec4(0, 1, 0, 1);
	//vec4 zColor = vec4(0, 0, 1, 1);
	vec4 xColor = texture(tileTex, objectPos.yz * 0.25);
	vec4 yColor = texture(tileTex, objectPos.zx * 0.25);
	vec4 zColor = texture(tileTex, objectPos.yx * 0.25);
	
	//vec4 zColor = vec4(objectPos.xy - ivec2(objectPos.xy), 0, 0);
	outColor = xColor * planarTextureCoeffictions.x + yColor * planarTextureCoeffictions.y + zColor * planarTextureCoeffictions.z;
	//outColor = vec4(1, 1, 1, 1);
}