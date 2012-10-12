#ifndef TESTCUBE_H
#define TESTCUBE_H
#include "BasicGraphicsComponent.h"

class TestCube: public BasicGraphicsComponent{
	TestCube()
		:BasicGraphicsComponent(NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, PM_GL_TRIANGLES)
	{}
	TestCube(char * vertex, char * tessalationControl, char * tessalationEvaluation,  char * geometry, char * pixel,  VertexInfo* vInfos, int numVIs, int numVerts, PrimitiveMode pmode)
		:BasicGraphicsComponent(vertex, tessalationControl, tessalationEvaluation, geometry, pixel,  vInfos,  numVIs,  numVerts, pmode)
	{}
public:
	static TestCube * CreateCube(){
		
		MVector<3>*	vertices = new MVector<3>[8];
		for(int i = 0; i < 8; ++i){
			vertices[i].SetValue(0, (float) ((i & 1) >> 0) - 0.5f);
			vertices[i].SetValue(1, (float) ((i & 2) >> 1) - 0.5f);
			vertices[i].SetValue(2, (float) ((i & 4) >> 2) - 0.5f);
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
		GLuint *mfaces = new GLuint[12 * 3];
		for(int i = 0; i < 12 * 3; ++i){
			mfaces[i] = faces[i];
		}
		MVector<3>*  normals = new MVector<3>[8];
		for(int i = 0; i < 8; ++i){
			normals[i].SetValue(0, 0);
			normals[i].SetValue(1, 0);
			normals[i].SetValue(2, 0);
		}
		for(int i = 0; i < 12; ++i){
			int v1 = faces[i * 3];
			int v2 = faces[i * 3 + 1];
			int v3 = faces[i * 3 + 2];
			MVector<3> vOne = vertices[v2] - vertices[v1];
			MVector<3> vTwo	= vertices[v3] - vertices[v1];
			MVector<3> crossed = vOne.Cross(vTwo);
			normals[v1] = normals[v1] + crossed;
			normals[v2] = normals[v2] + crossed;
			normals[v3] = normals[v3] + crossed;
		}
		for(int i = 0; i < 8; ++i){
			normals[i].Normalize();
		}
		
		VertexInfo * vIs = new VertexInfo[4];
		vIs[0].Set("Indices", false	, 12 * 3 * sizeof(GLuint)	, mfaces, VertexInfo::U_GL_STATIC_DRAW,
			0,	0,	VertexInfo::DT_GL_UNSIGNED_INT,	false, 0, 0);

		vIs[1].Set("VertexPosition"	, true	, 8 * 3 * sizeof(GLfloat)	, vertices,	VertexInfo::U_GL_STATIC_DRAW,
			1,	3,	VertexInfo::DT_GL_FLOAT,	false, 0, 0 );
		vIs[2].Set("Color"			, true	, 8 * 3 * sizeof(GLfloat)	, vertices,	VertexInfo::U_GL_STATIC_DRAW,
			2,	3,	VertexInfo::DT_GL_FLOAT,	false, 0, 0 );
		vIs[3].Set("Normal"			, true	, 8 * 3 * sizeof(GLfloat)	, normals,	VertexInfo::U_GL_STATIC_DRAW,
			3,	3,	VertexInfo::DT_GL_FLOAT,	false, 0, 0 );

		return new TestCube("CubeVertex.glsl", NULL, NULL, NULL, "CubePixel.glsl", vIs, 4, 8, PM_GL_TRIANGLES);
		
	}
	virtual void EntitySpecificShaderSetup(){
		shader->SetUniformMatrix("model", Matrix<4, 4>::Identity());
	}

};

#endif