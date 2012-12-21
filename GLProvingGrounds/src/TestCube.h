#ifndef TESTCUBE_H
#define TESTCUBE_H
#include "BasicGraphicsComponent.h"
#include "Texture.h"

class TestCube: public BasicGraphicsComponent{

	Texture* tex;
	TestCube()
		:BasicGraphicsComponent(NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, PM_GL_TRIANGLES)
	{}
	TestCube(char * vertex, char * tessalationControl, char * tessalationEvaluation,  char * geometry, char * pixel,  VertexInfo* vInfos, int numVIs, int numElements, PrimitiveMode pmode, Texture* tex)
		:BasicGraphicsComponent(vertex, tessalationControl, tessalationEvaluation, geometry, pixel,  vInfos,  numVIs,  numElements, pmode), tex(tex)
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
			0, 2, 3,
			0, 3, 1,
			4, 6, 2,
			4, 2, 0,
			5, 7, 6,
			5, 6, 4,
			1, 3, 7,
			1, 7, 5,
			3, 2, 6,
			3, 6, 7,
			1, 5, 4,
			1, 4, 0
		};
		GLuint *mfaces = new GLuint[12 * 3];
		for(int i = 0; i < 12 * 3; ++i){
			mfaces[i] = faces[i];
		}
		MVector<3>*  normals = new MVector<3>[8];
		memset(normals, 0, sizeof(MVector<3>) * 8);
		for(int i = 0; i < 6; ++i){
			int v1 = faces[i * 6];
			int v2 = faces[i * 6 + 1];
			int v3 = faces[i * 6 + 2];
			int v4 = faces[i * 6 + 5];
			MVector<3> vOne = vertices[v2] - vertices[v1];
			MVector<3> vTwo	= vertices[v3] - vertices[v1];
			MVector<3> crossed = vOne.Cross(vTwo);
			normals[v1] = normals[v1] + crossed;
			normals[v2] = normals[v2] + crossed;
			normals[v3] = normals[v3] + crossed;
			normals[v4] = normals[v4] + crossed;
		}
		for(int i = 0; i < 8; ++i){
			normals[i].Normalize();
		}
		
		Texture * tex = new Texture(Texture::TT_GL_TEXTURE_2D, GL_RGB32F);

		tex->SetParamInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		tex->SetParamInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		tex->SetParamInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
		tex->SetParamInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
		tex->SetParamInt(GL_TEXTURE_WRAP_R, GL_REPEAT);

		Texture::TexData data;
		memset(&data, 0, sizeof(Texture::TexData));
		
		float d[64][64][3];
		for(int x = 0; x < 64 ; ++x){
			for(int y = 0; y < 64; ++y){
				d[x][y][0] = (float)x/64.0f;
				d[x][y][1] = (float)y/64.0f;
				d[x][y][2] = (float)(x + y) / 128.0f;
			}
		}
		
		data.height = 64;
		data.width	= 64;
		data.level	= 0;

		data.type = GL_FLOAT;
		data.format = GL_RGB;
		data.data = &d;

		tex->GiveData(data);
		
		VertexInfo * vIs = new VertexInfo[4];
		vIs[0].Set("Indices", false	, 12 * 3 * sizeof(GLuint)	, mfaces, VertexInfo::U_GL_STATIC_DRAW,
			0,	0,	VertexInfo::DT_GL_UNSIGNED_INT,	false, 0, 0);

		vIs[1].Set("VertexPosition"	, true	, 8 * 3 * sizeof(GLfloat)	, vertices,	VertexInfo::U_GL_STATIC_DRAW,
			1,	3,	VertexInfo::DT_GL_FLOAT,	false, 0, 0 );
		vIs[2].Set("Color"			, true	, 8 * 3 * sizeof(GLfloat)	, vertices,	VertexInfo::U_GL_STATIC_DRAW,
			2,	3,	VertexInfo::DT_GL_FLOAT,	false, 0, 0 );
		vIs[3].Set("Normal"			, true	, 8 * 3 * sizeof(GLfloat)	, normals,	VertexInfo::U_GL_STATIC_DRAW,
			3,	3,	VertexInfo::DT_GL_FLOAT,	false, 0, 0 );

		return new TestCube("Shaders/CubeVertex.glsl", NULL, NULL, NULL, "Shaders/CubePixel.glsl", vIs, 4, 12 * 3, PM_GL_TRIANGLES, tex);
		
	}
	virtual void EntitySpecificShaderSetup(){
		shader->SetUniformMatrix("model", Matrix<4, 4>::Identity());
		tex->MakeActive(0);
	}
	virtual void Render(int pass){
		
		BasicGraphicsComponent::Render(pass);
	}

};

#endif