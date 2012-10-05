#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec3 VertexPosition;
in vec3 Color;
in vec3 Normal;

out vec4 ocolor;

void main(void){
	gl_Position =  proj*view*model * vec4(VertexPosition, 1.0);
	ocolor = vec4(Color, 1.0);
}