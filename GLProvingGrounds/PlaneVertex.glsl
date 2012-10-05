#version 150

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float offset;
uniform vec3 lightPosition;
uniform float width;
uniform float height;
uniform vec3 camPos;
in vec3 VertexPosition;
in vec3 Color;
in vec3 Normal;



out vec4 ocolor;
// out vec3 onorm;
out vec3 lightDir;
out vec2 coefficients;
out vec3 camDir;

void main(void){
	float mX = VertexPosition.x;
	float mZ = VertexPosition.z;
	float coefficientX = mX + offset;
	float coefficientZ = mZ + offset;
	vec3 myPos = vec3(mX, sin(coefficientX) * sin(coefficientZ)* 0.25, mZ);
//	vec3 leftPos =vec3(mX - width, sin(coefficientX- width) * sin(coefficientZ)* 0.25, mZ);
//	vec3 upPos =vec3(mX , sin(coefficientX) * sin(coefficientZ+ height)* 0.25, mZ+ height);
//	vec3 rightPos =vec3(mX + width, sin(coefficientX+ width) * sin(coefficientZ)* 0.25, mZ);
//	vec3 downPos = vec3(mX , sin(coefficientX) * sin(coefficientZ- height)* 0.25, mZ- height);
//	onorm = cross((upPos - leftPos), (myPos - leftPos));
//	onorm += cross((upPos - myPos), ( rightPos- myPos));
//	onorm += cross((downPos - leftPos), ( downPos- myPos));
//	onorm += cross((downPos - myPos), ( downPos- rightPos));
//	onorm = transpose(inverse(model)) * vec4(onorm, 0.0);
//	onorm = normalize(onorm);
	
	
	
	gl_Position = proj * view * model   * vec4(myPos, 1.0);
	ocolor = vec4(Color, 1.0);
	lightDir = lightPosition - (model * vec4(myPos, 1.0)).xyz;
	lightDir = normalize(lightDir);
	coefficients = vec2(coefficientX, coefficientZ);
	camDir = camPos - (model * vec4(myPos, 1.0)).xyz;
	camDir = normalize(camDir);
}