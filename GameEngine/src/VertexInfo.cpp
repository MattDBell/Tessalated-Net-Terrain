#include "VertexInfo.h"
#include "Macros.h"
#include <GL\glew.h>
void VertexInfo::Set(	char* name, bool isArrayBuffer, unsigned int dataBufferSize, void * dataBufferPtr, Usage usage,
	int  elementIndex, int sizePerVertexAttrib, DataType type, bool isNormalized, int stride, void* offset){
		this->name = name;
		bIsArrayBuffer = isArrayBuffer;
		this->dataBufferSize = dataBufferSize;
		dataBuffer = std::shared_ptr<void>(dataBufferPtr);
		this->usage = usage;
		this->iElementIndex = elementIndex;
		this->sizePerVertexAttrib = sizePerVertexAttrib;
		this->type = type;
		this->bIsNormalized = isNormalized;
		this->stride = stride;
		this->pointer = offset;
}
void VertexInfo::BindAttribLocation(int shaderBuffer){
	if(!bIsArrayBuffer)
		return;
	GLCALL(glBindAttribLocation(shaderBuffer, iElementIndex, name));
}
void VertexInfo::Bind(){
	GLCALL(glBindBuffer(bIsArrayBuffer ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, iBuffer));
}
void VertexInfo::BufferData(){
	GLCALL(glBufferData(bIsArrayBuffer ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER,
		dataBufferSize, dataBuffer.get(), usage));
}
void VertexInfo::VertexAttribPointer(){
	if(!bIsArrayBuffer)
		return;
	GLCALL(glVertexAttribPointer(iElementIndex, sizePerVertexAttrib, type, bIsNormalized ? GL_TRUE : GL_FALSE, stride, pointer));
}
void VertexInfo::Enable(){
	if(!bIsArrayBuffer)
		return;
	GLCALL(glEnableVertexAttribArray(iElementIndex));
}
void VertexInfo::Initialize(){
	Bind();
	BufferData();
	Enable();
	VertexAttribPointer();
}
VertexInfo::~VertexInfo(){
	GLCALL(glDeleteBuffers(1, &iBuffer));
}