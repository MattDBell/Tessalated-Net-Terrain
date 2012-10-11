#ifndef TESTCUBE_H
#define TESTCUBE_H
#include "BasicGraphicsComponent.h"

class TestCube: public BasicGraphicsComponent{
	TestCube();
public:
	static TestCube * CreateCube(){
		
		MVector<3>	vertices[8];
		for(int i = 0; i < 8; ++i){
			vertices[i].SetValue(0, (i & 1) >> 0);
			vertices[i].SetValue(0, (i & 2) >> 1);
			vertices[i].SetValue(0, (i & 4) >> 2);
		}
		//Must be a nice algorithm for this
		GLuint faces[12 * 3] = {
			0, 2, 1,
			1, 2, 3,
			1, 3, 5,
			5, 3, 7,
			0, 3, 4,
			4, 3, 5,
			0, 4, 2,
			4, 6, 2,
			4, 6, 5,
			5, 6, 7,
			0, 6, 3,
			6, 7, 3
		};
		MVector<3> yo = {3.0f, 3.0f, 3.0f};
		MVector<3>  normals[8];
		for(int i = 0; i < 8; ++i){
			normals[i].SetValue(0, 0);
			normals[i].SetValue(1, 0);
			normals[i].SetValue(2, 0);
		}
		for(int i = 0; i < 12; ++i){
			MVector<3> vOne = vertices[faces[i * 3 + 1]] - vertices[faces[i * 3]];
			MVector<3> vTwo	= vertices[faces[i * 3 + 2]] - vertices[faces[i * 3]];
			normals[i] = normals[i] + vOne.Cross(vTwo);
		}
		for(int i = 0; i < 8; ++i){
			normals[i].Normalize();
		}
		
		VertexInfo vIs[2];
		vIs[0].Set("Indices", false	, 12 * 3 * sizeof(GLuint)	, faces,	VertexInfo::Usage::U_GL_STATIC_DRAW,
			0,	0,	VertexInfo::DataType::DT_GL_UNSIGNED_INT,	false, 0, 0);

		vIs[1].Set("VertexPosition"	, true	, 8 * 3 * sizeof(GLfloat)	, vertices,	VertexInfo::Usage::U_GL_STATIC_DRAW,
			1,	3 * sizeof(float),	VertexInfo::DataType::DT_GL_FLOAT,	false, 0, 0 );
		vIs[2].Set("Color"			, true	, 8 * 3 * sizeof(GLfloat)	, vertices,	VertexInfo::Usage::U_GL_STATIC_DRAW,
			2,	3 * sizeof(float),	VertexInfo::DataType::DT_GL_FLOAT,	false, 0, 0 );
		vIs[3].Set("Normal"			, true	, 8 * 3 * sizeof(GLfloat)	, normals,	VertexInfo::Usage::U_GL_STATIC_DRAW,
			3,	3 * sizeof(float),	VertexInfo::DataType::DT_GL_FLOAT,	false, 0, 0 );
		
	}

};

#endif