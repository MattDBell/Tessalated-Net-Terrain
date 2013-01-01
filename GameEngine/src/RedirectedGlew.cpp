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
void rglBufferData(unsigned int type, ptrdiff_t size, void * data, unsigned int usage)
{
	GLCALL(glBufferData(type, size, data, usage));
}
void rglBindBufferRange(unsigned int type, unsigned int bindingIndex, unsigned int uboIndex, ptrdiff_t offset, ptrdiff_t size)
{
	GLCALL(glBindBufferRange(type, bindingIndex, uboIndex, offset, size));
}
unsigned int rglGetUniformBlockIndex(unsigned int programBuffer, char* name)
{
#if _DEBUG
	unsigned int buff = glGetUniformBlockIndex(programBuffer, name);
	CheckErrors();
	return buff;
#else
	return glGetUniformBlockIndex(programBuffer, name);
#endif
}
void rglUniformBlockBinding(unsigned int programBuffer, unsigned int uniformBlockIndex, unsigned int uBOBindingIndex)
{
	GLCALL(glUniformBlockBinding(programBuffer, uniformBlockIndex, uBOBindingIndex));
}
void rglBufferSubData(unsigned int bufferEnum, ptrdiff_t offset, ptrdiff_t size, void * data)
{
	GLCALL(glBufferSubData(bufferEnum, offset, size, data));
}