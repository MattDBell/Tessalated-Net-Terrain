#include "GraphicsComponent.h"
#include "GL\glew.h"
#include "GL\wglew.h"
#include "Camera.h"


GraphicsComponent::GraphicsComponent(VertexInfo* vInfos, int numVIs, int numVerts, PrimitiveMode pmode)
	: vInfos(vInfos), numVInfo(numVIs), numVerts(numVerts), primMode(pmode)
{}
GraphicsComponent::~GraphicsComponent()
{}

//This should be refactored in order to allow more Per Vertex information.
void GraphicsComponent::Initialize(){
	
	//Tell the shader where the Vertex properties are going to be stored.  (Which array is which)
	
	//Generate the Vertex Array Object then Bind it.  VAOs are used to store
	//a series of ArrayBuffers and an Element Array Buffer for ease of drawing
	glGenVertexArrays(1, &vao_ID);
	glBindVertexArray(vao_ID);

	//Generate other buffers (vertex, color, normal)

	GLuint* buffers = new GLuint[numVInfo];
	glGenBuffers(numVInfo, buffers);

	for(int i = 0; i < numVInfo; ++i){
		vInfos[i].iBuffer = buffers[i];
		vInfos[i].Initialize();
	}
	//Unbind VAO
	glBindVertexArray(0);

}
void GraphicsComponent::EntitySpecificShaderSetup(){
	return;
}
void GraphicsComponent::Render(int){
	EntitySpecificShaderSetup();
	GLCALL(glBindVertexArray(vao_ID));
	//GLCALL(glDrawArrays(primMode, 0, numVerts));
	GLCALL(glDrawElements(primMode, numVerts, GL_UNSIGNED_INT, (void*)0));
	GLCALL(glBindVertexArray(0));
}
