#include "GraphicsComponent.h"
#include "Camera.h"
#include <GL\glew.h>


GraphicsComponent::GraphicsComponent(VertexInfo* vInfos, int numVIs, int numElements, PrimitiveMode pmode)
	: vInfos(vInfos), numVInfo(numVIs), numElements(numElements), primMode(pmode), usesElements(false)
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
		if(!vInfos[i].bIsArrayBuffer){
			usesElements = true;
		}
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
	if(!usesElements){
		GLCALL(glDrawArrays(primMode, 0, numElements));
	}
	else{
		GLCALL(glDrawElements(primMode, numElements, GL_UNSIGNED_INT, (void*)0));
	}
	GLCALL(glBindVertexArray(0));
}
