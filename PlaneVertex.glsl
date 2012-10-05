#version 150

uniform mat4 modelViewProj;

in vec3 VertexPosition;
in vec3 Color;
in vec3 Normal;

out vec4 ocolor;

void main(void){
	gl_Position = modelViewProj * vec4(VertexPosition, 1.0);
	ocolor = vec4(Color, 1.0);
}