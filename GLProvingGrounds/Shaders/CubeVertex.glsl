#version 420

layout ( std140 ) uniform CamMats{
	mat4 view;
	mat4 proj;
};

uniform mat4 model;

in vec3 VertexPosition;
in vec3 Color;
in vec3 Normal;

out vec4 ocolor;
out vec4 world;
out vec4 normal;

void main(void){
	normal = model * vec4(Normal, 0);
	world = model * vec4(VertexPosition, 1.0);
	gl_Position = proj * view * world;// a
	ocolor = vec4(Color, 1.0) + vec4(0.5, 0.5, 0.5, 0);
}