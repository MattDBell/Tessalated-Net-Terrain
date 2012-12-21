#include "Macros.h"
#include <GL/glew.h>

void rglDeleteBuffers(int num, unsigned int *buffers )
{
	GLCALL(glDeleteBuffers(num, buffers));
}
void rglGenBuffers(int num, unsigned int* buffers)
{
	GLCALL(glGenBuffers(num, buffers));
}
void rglBindBuffer(unsigned int type, unsigned int buffer)
{
	GLCALL(glBindBuffer(type, buffer));
}
void rglBufferData(unsigned int type, size_t size, void * data, unsigned int usage)
{
	GLCALL(glBufferData(type, size, data, usage));
}
void rglBindBufferRange(unsigned int type, unsigned int bindingIndex, unsigned int uboIndex, unsigned int offset, size_t size)
{
	GLCALL(glBindBufferRange(type, bindingIndex, uboIndex, offset, size));
}