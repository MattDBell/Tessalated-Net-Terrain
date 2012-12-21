#ifndef MACROS_H
#define MACROS_H

void CheckErrors();

#ifdef _DEBUG
#define GLCALL( function ) function; CheckErrors()
#else
#define GLCALL( function ) function
#endif


#endif