#include "Texture.h"

Texture::Texture(TexType t, GLint format)
		:type(t), format(format)
{
	GLCALL(glGenTextures(1, &buffer));
}

void Texture::SetStorage(GLsizei width, GLsizei height, GLsizei depth, GLint levels){
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

void Texture::GiveData(TexData toGive){
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

void Texture::SetParamFloat(GLenum pName, GLfloat param){
	GLCALL(glBindTexture(type, buffer));
	GLCALL(glTexParameterf(type, pName, param));
}

void Texture::SetParamInt(GLenum pName, GLint param) {

	GLCALL( glBindTexture(type, buffer) );

	GLCALL( glTexParameteri(type, pName, param) );
}

void Texture::SetParamFloatVector(GLenum pName, GLfloat * param){
	GLCALL( glBindTexture(type, buffer) );
	GLCALL( glTexParameterfv(type, pName, param) );
}

void Texture::SetParamFloatVector(GLenum pName, GLint * param){
	GLCALL( glBindTexture(type, buffer) );
	GLCALL( glTexParameteriv(type, pName, param) );
}

void Texture::SetParamIntVector(GLenum pName, GLint * param){
	GLCALL( glBindTexture(type, buffer) );
	GLCALL( glTexParameterIiv(type, pName, param) );
}

void Texture::SetParamIntVector(GLenum pName, GLuint* param){
	GLCALL( glBindTexture(type, buffer) );
	GLCALL( glTexParameterIuiv(type, pName, param) );
}

void Texture::GenerateMipMaps(){
	GLCALL(glBindTexture(type, buffer));
	GLCALL(glGenerateMipmap(type));
}

void Texture::MakeActive(unsigned int slot){
	GLCALL(glBindTexture(type, buffer));
	if(slot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS){
		boundTo = GL_TEXTURE0 + slot;
		GLCALL(glActiveTexture(boundTo));
	}
}