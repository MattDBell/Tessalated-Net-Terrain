layout ( std140 ) uniform CamMats{
	mat4 view;
	mat4 proj;
};

uniform mat4 model;

in vec4 position;
out vec4 world;
out vec4 right;
out vec4 up;
out vec4 forward;


void main(void){
	world	= model * position;
	right	= model * vec4(1, 0, 0, 0);
	up		= model * vec4(0, 1, 0, 0);
	forward	= model * vec4(0, 0, 1, 0);
	output.pos = mul(input.position, mul(ObjectTransform,WorldViewProjection )); // move it
}