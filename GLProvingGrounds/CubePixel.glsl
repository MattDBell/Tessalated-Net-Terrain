#version 400

in vec4 ocolor;
out vec4 outColor;

void main(void){
	outColor = ocolor + vec4(0.5, 0.5, 0.5, 0);
}