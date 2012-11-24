#version 420

/*
Marching Cube algorithm works but!
normal must be in world space
world position must be passed to pixel so it can figure out light direction
object position must be passed to pixel so it can figure out texCoords
Do model transform later...?

Possibility One:

Vertex passes transformed right up forward position and uvwcoords
Geometry interpolates both position (view proj) and uvwcoords depending on texture then passes to pixel
pixel does texturing

Possibility Two:

Vertex passes raw vertex position
Geometry does interpolation in object space then passes view proj, world and object position to pixel shader

*/

layout ( std140 ) uniform CamMats{
	mat4 view;
	mat4 proj;
};



in vec3 VertexPosition;

//out VertexData {
//	vec4 uvwCoords;
//	vec4 right;
//	vec4 up;
//	vec4 forward;
//} outData;


void main(void){
	vec4 position		= vec4(VertexPosition, 1);
	gl_Position			= position;
//	outData.right		= model * vec4(1, 0, 0, 0);
//	outData.up			= model * vec4(0, 1, 0, 0);
//	outData.forward		= model * vec4(0, 0, 1, 0);
//	outData.uvwCoords	= vec4(VertexPosition, 0);
}
