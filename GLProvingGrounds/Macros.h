#ifndef MACROS_H
#define MACROS_H
#include <GL\glew.h>
#include <GL\wglew.h>
#include <stdio.h>

#ifdef _DEBUG
#define GLCALL( function ) function; CheckErrors()
#else
#define GLCALL( function ) function
#endif
void CheckErrors(){
	GLenum result = glGetError();
	while(result != GL_NO_ERROR )
	{
		char * error;
		switch(result){
			case GL_INVALID_ENUM:
				error = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				error = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				error = "GL_INVALID_OPERATION";
				break;
			case GL_STACK_OVERFLOW:
				error = "GL_STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				error = "GL_STACK_UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:
				error = "GL_OUT_OF_MEMORY";
				break;
		}
		printf(error);
		result = glGetError();
	}
}

#endif