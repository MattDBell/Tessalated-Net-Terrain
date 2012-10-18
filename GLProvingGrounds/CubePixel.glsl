#version 420

in vec4 ocolor;
out vec4 outColor;

layout(binding = 0) uniform sampler2D samp;

void main(void){
	outColor = ocolor + vec4(0.5, 0.5, 0.5, 0);
	outColor = texture(samp, outColor.xy);
}