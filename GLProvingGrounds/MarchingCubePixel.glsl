#version 420

layout( binding = 2 ) uniform sampler2D		xTileTex;
layout( binding = 3 ) uniform sampler2D		yTileTex;
layout( binding = 4 ) uniform sampler2D		zTileTex;
layout( binding = 5 ) uniform sampler2D		xTileNorm;
layout( binding = 6 ) uniform sampler2D		yTileNorm;
layout( binding = 7 ) uniform sampler2D		zTileNorm;
layout( binding = 8 ) uniform sampler2D		xTileSpec;
layout( binding = 9 ) uniform sampler2D		yTileSpec;
layout( binding = 10 ) uniform sampler2D	zTileSpec;




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

	float normalSum = abs(normal.x) + abs(normal.y) + abs(normal.z);
	vec3 planarTextureCoefficients = vec3(  abs(normal.x)/normalSum, abs(normal.y)/normalSum, abs(normal.z)/normalSum );
	
	vec2 xTex = objectPos.yz * 0.25;
	vec2 yTex = objectPos.zx * 0.25;
	vec2 zTex = objectPos.yx * 0.25;

	vec4 xColor = texture(xTileTex, xTex);
	vec4 yColor = texture(yTileTex, yTex);
	vec4 zColor = texture(zTileTex, zTex);

	vec2 xBump = texture(xTileNorm, xTex).xy;
	vec2 yBump = texture(yTileNorm, yTex).xy;
	vec2 zBump = texture(zTileNorm, zTex).xy;

	//vec3 xTan = vec3(normal.z, normal.y, -normal.x);
	//vec3 xCoTan = vec3(normal.y, -normal.x, normal.z);
	//
	//vec3 yTan = vec3(normal.x, normal.z, -normal.y);
	//vec3 yCoTan = vec3(-normal.y, normal.x, normal.z);
	//
	//vec3 zTan = vec3(-normal.z, normal.y, normal.x);
	//vec3 zCoTan = vec3(normal.x, -normal.z, normal.y);
	normal = normalize(normal + vec3(0, xBump.x, xBump.y) * planarTextureCoefficients.xxx + vec3(yBump.y, 0, yBump.x) *planarTextureCoefficients.yyy + vec3(zBump.x, zBump.y, 0) * planarTextureCoefficients.zzz);

	vec4 diffuseColor = vec4(0, 0, 0, 1);
	for(int i = 0; i < 10; ++i){
		vec4 lightDir = position[i] - worldPos;
		float intensity = clamp(dot(normal, normalize(lightDir).xyz), 0.0, 1.0);
		diffuseColor +=  color[i] *intensity;
	}
	
	//vec4 xColor = vec4(1, 0, 0, 1);
	//vec4 yColor = vec4(0, 1, 0, 1);
	//vec4 zColor = vec4(0, 0, 1, 1);
	
	
	//vec4 zColor = vec4(objectPos.xy - ivec2(objectPos.xy), 0, 0);
	outColor = xColor * planarTextureCoefficients.x + yColor * planarTextureCoefficients.y + zColor * planarTextureCoefficients.z;
	outColor *= diffuseColor;
}