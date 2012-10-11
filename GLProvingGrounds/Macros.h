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
void CheckErrors();

#endif