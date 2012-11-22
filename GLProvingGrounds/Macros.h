#ifndef MACROS_H
#define MACROS_H
#include <GL\glew.h>
#include <GL\wglew.h>
#include <stdio.h>

void CheckErrors();

#ifdef _DEBUG
#define GLCALL( function ) function; CheckErrors()
#else
#define GLCALL( function ) function
#endif


#endif