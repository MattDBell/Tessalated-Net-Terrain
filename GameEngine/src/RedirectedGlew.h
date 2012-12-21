
void rglDeleteBuffers(int num, unsigned int *buffers );
//GLCALL(glDeleteBuffers(1, &uboIndex));
void rglGenBuffers(int num, unsigned int* buffers);
//GLCALL(glGenBuffers(1, &uboIndex));
void rglBindBuffer(unsigned int type, unsigned int buffer);
//GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uboIndex));
void rglBufferData(unsigned int type, size_t size, void * something, unsigned int usage);
//GLCALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(dataStructure), NULL, GL_STREAM_DRAW));
void rglBindBufferRange(unsigned int type, unsigned int bindingIndex, unsigned int uboIndex, unsigned int offset, size_t size);
//GLCALL(glBindBufferRange(GL_UNIFORM_BUFFER, uBOBindingIndex, uboIndex, 0, sizeof(dataStructure)));
//unsigned int uniformBlockIndex = GLCALL(glGetUniformBlockIndex(programBuffer, name));
//GLCALL(glUniformBlockBinding(programBuffer, uniformBlockIndex, uBOBindingIndex));
//GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uboIndex));
//GLCALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
//GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uboIndex));
//GLCALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(dataStructure), data, GL_STREAM_DRAW));
