#include <stdint.h>

#ifndef GL_UNIFORM_BUFFER
#define GL_UNIFORM_BUFFER 0x8A11
#endif
#ifndef GL_INVALID_INDEX
#define GL_INVALID_INDEX 0xffffffff
#endif
#ifndef GL_STREAM_DRAW
#define GL_STREAM_DRAW 0x88e0
#endif

void rglDeleteBuffers(int num, unsigned int *buffers );
//GLCALL(glDeleteBuffers(1, &uboIndex));
void rglGenBuffers(int num, unsigned int* buffers);
//GLCALL(glGenBuffers(1, &uboIndex));
void rglBindBuffer(unsigned int type, unsigned int buffer);
//GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uboIndex));
void rglBufferData(unsigned int type, ptrdiff_t size, void * data, unsigned int usage);
//GLCALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(dataStructure), NULL, GL_STREAM_DRAW));
void rglBindBufferRange(unsigned int type, unsigned int bindingIndex, unsigned int uboIndex, ptrdiff_t offset, ptrdiff_t size);
//GLCALL(glBindBufferRange(GL_UNIFORM_BUFFER, uBOBindingIndex, uboIndex, 0, sizeof(dataStructure)));
unsigned int rglGetUniformBlockIndex(unsigned int programBuffer, char* name);
//unsigned int uniformBlockIndex = GLCALL(glGetUniformBlockIndex(programBuffer, name));
void rglUniformBlockBinding(unsigned int programBuffer, unsigned int uniformBlockIndex, unsigned int uBOBindingIndex);
//GLCALL(glUniformBlockBinding(programBuffer, uniformBlockIndex, uBOBindingIndex));
void rglBufferSubData(unsigned int bufferEnum, ptrdiff_t offset, ptrdiff_t size, void * data);
//GLCALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
