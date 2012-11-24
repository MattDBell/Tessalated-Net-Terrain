#version 420

layout( binding = 2 ) uniform sampler tileTex;
//layout( binding = 3 ) uniform sampler tileNorm;

in PixelData{
	vec3 normal;
	vec4 objectPos;
	vec4 worldPos;
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
	float normalSum = normal.x + normal.y + normal.z;
	vec3 planarTextureCoeffictions = vec3(  normal.x/normalSum, normal.y/normalSum, normal.z/normalSum );
	vec4 xColor = Tex(tileTex, objectPos.yz);
	vec4 yColor = Tex(tileTex, objectPos.zx);
	vec4 zColor = Tex(tileTex, objectPos.xy);
	outColor = xColor * planarTextureCoeffictions.x + yColor * planarTextureCoeffictions.y + zColor * planarTextureCoeffictions.z;
	outColor = vec4(0.5, 0.5, 0.5, 1) * diffuseColor;
}