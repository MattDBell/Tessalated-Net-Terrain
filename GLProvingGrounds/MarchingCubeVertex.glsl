layout ( std140 ) uniform CamMats{
	mat4 view;
	mat4 proj;
};

uniform mat4 model;

in vec3 VertexPosition;

out VertexData {
	vec4 uvwCoords;
	vec4 right;
	vec4 up;
	vec4 forward;
} outData;


void main(void){
	vec4 position		= vec4(VertexPosition, 1);
	gl_Position			= model * position;
	outData.right		= model * vec4(1, 0, 0, 0);
	outData.up			= model * vec4(0, 1, 0, 0);
	outData.forward		= model * vec4(0, 0, 1, 0);
	outData.uvwCoords	= vec4(VertexPosition, 0);
}