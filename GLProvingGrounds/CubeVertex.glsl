#version 400

layout ( std140 ) uniform CamMats{
	mat4 view;
	mat4 proj;
} cMat;

uniform mat4 model;

in vec3 VertexPosition;
in vec3 Color;
in vec3 Normal;

out vec4 ocolor;

void main(void){
	gl_Position =  vec4(VertexPosition, 1 );// cMat.proj * cMat.view * model * vec4(VertexPosition, 1.0);
	//gl_Position /= gl_Position.w;
	ocolor = vec4(Color, 1.0);
}