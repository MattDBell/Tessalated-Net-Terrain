#include "Macros.h"
#include "Texture.h"
#include <GL/glew.h>
Texture::Texture(TexType t, int format)
		:type(t), format(format)
{
	GLCALL(glGenTextures(1, &buffer));
}

void Texture::SetStorage(int width, int height, int depth, int levels){
	switch(type){
		case TT_GL_TEXTURE_1D:
			GLCALL(glTexStorage1D(type, levels, format, width));
			break;
		case TT_GL_TEXTURE_2D:
		case TT_GL_TEXTURE_1D_ARRAY:
		case TT_GL_TEXTURE_RECTANGLE:
		case TT_GL_TEXTURE_CUBE_MAP:
			GLCALL(glTexStorage2D(type, levels, format, width, height));
			break;

		case TT_GL_TEXTURE_3D:
		case TT_GL_TEXTURE_2D_ARRAY:
		case TT_GL_TEXTURE_CUBE_MAP_ARRAY:
			GLCALL(glTexStorage3D(type, levels, format, width, height, depth));
			break;
	}
}

void Texture::GiveData(TexData &toGive){
	GLCALL(glBindTexture(type, buffer));
	switch(type){
	case TT_GL_TEXTURE_1D:
		GLCALL(glTexImage1D(type, toGive.level, format, toGive.width, 0, toGive.format, toGive.type, toGive.data));
		break;
	case TT_GL_TEXTURE_2D:
	case TT_GL_TEXTURE_1D_ARRAY:
	case TT_GL_TEXTURE_RECTANGLE:
		GLCALL(glTexImage2D(type, toGive.level, format, toGive.width, toGive.height, 0, toGive.format, toGive.type, toGive.data));
		break;
	case TT_GL_TEXTURE_CUBE_MAP:
		GLCALL(glTexImage2D(toGive.target, toGive.level, format, toGive.width, toGive.height, 0, toGive.format, toGive.type, toGive.data));
		break;
	case TT_GL_TEXTURE_3D:
	case TT_GL_TEXTURE_2D_ARRAY:
	case TT_GL_TEXTURE_CUBE_MAP_ARRAY:
		GLCALL(glTexImage3D(type, toGive.level, format, toGive.width, toGive.height, toGive.depth, 0, toGive.format, toGive.type, toGive.data));
		break;
	}
	GLCALL(glBindTexture(type, 0));
}

void Texture::SetParamFloat(unsigned int pName, float param){
	GLCALL(glBindTexture(type, buffer));
	GLCALL(glTexParameterf(type, pName, param));
}

void Texture::SetParamInt(unsigned int pName, int param) {

	GLCALL( glBindTexture(type, buffer) );

	GLCALL( glTexParameteri(type, pName, param) );
}

void Texture::SetParamFloatVector(unsigned int pName, float* param){
	GLCALL( glBindTexture(type, buffer) );
	GLCALL( glTexParameterfv(type, pName, param) );
}

void Texture::SetParamFloatVector(unsigned int pName, int * param){
	GLCALL( glBindTexture(type, buffer) );
	GLCALL( glTexParameteriv(type, pName, param) );
}

void Texture::SetParamIntVector(unsigned int pName, int * param){
	GLCALL( glBindTexture(type, buffer) );
	GLCALL( glTexParameterIiv(type, pName, param) );
}

void Texture::SetParamIntVector(unsigned int pName, unsigned int* param){
	GLCALL( glBindTexture(type, buffer) );
	GLCALL( glTexParameterIuiv(type, pName, param) );
}

void Texture::GenerateMipMaps(){
	GLCALL(glBindTexture(type, buffer));
	GLCALL(glGenerateMipmap(type));
}

void Texture::MakeActive(unsigned int slot){
	if(slot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS){
		boundTo = GL_TEXTURE0 + slot;
		GLCALL(glActiveTexture(boundTo));
		GLCALL(glBindTexture(type, buffer));
	}
	
	
}