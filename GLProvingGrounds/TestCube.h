#ifndef TESTCUBE_H
#define TESTCUBE_H
#include "BasicGraphicsComponent.h"

class TestCube: public BasicGraphicsComponent{
	TestCube();
public:
	static TestCube * CreateCube(){
		
		GLfloat		vertices[8*3];
		for(int i = 0; i < 8; ++i){
			vertices[i*3]			= (i & 1) >> 0;
			vertices[i*3 +1]		= (i & 2) >> 1;
			vertices[i*3 +2]		= (i & 4) >> 2;
		}

		//Must be a nice algorithm for this
		GLuint		faces[12 * 3];
		for(int i = 0; i < 12; ++i){
			faces[i*3] = 
		}
		
		VertexInfo vIs[2];
		vIs[0].Set("Indices", false, 12 * 3, NULL, VertexInfo::Usage::U_GL_STATIC_DRAW, 0, 0, VertexInfo::DataType::DT_GL_UNSIGNED_INT, false, 0, 0);
		vIs[1].Set("Position", true, 8, NULL, U_GL_STATIC_DRAW, 1, 3 * sizeof(float), VertexInfo::DataType::DT_GL_FLOAT, false, 0, 0);
		
	}

};

#endif