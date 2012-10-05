#ifndef TEXTURE_H
#define TEXTURE_H
#include "Macros.h"
class Texture{
public:
	enum TexType{
		TT_GL_TEXTURE_1D					=0x0DE0,
		TT_GL_TEXTURE_2D					=0x0DE1,
		TT_GL_TEXTURE_3D					=0x806F,
		TT_GL_TEXTURE_RECTANGLE				=0x84F5,
		TT_GL_TEXTURE_BUFFER				=0x8C2A,
		TT_GL_TEXTURE_CUBE_MAP				=0x8513,
		TT_GL_TEXTURE_1D_ARRAY				=0x8C18,
		TT_GL_TEXTURE_2D_ARRAY				=0x8C1A,
		TT_GL_TEXTURE_CUBE_MAP_ARRAY		=0x9009,
		TT_GL_TEXTURE_2D_MULTISAMPLE		=0x9100,
		TT_GL_TEXTURE_2D_MULTISAMPLE_ARRAY	=0x9102
	};

	struct TexData{
		GLenum		target;
		GLint		level;
		GLsizei		width;
		GLsizei		height;
		GLsizei		depth;
		GLenum		format;
		GLenum		type;
		void*		data;
	};

	
	Texture(TexType t, GLint f)
		:type(t), format(f)
	{
		GLCALL(glGenTextures(1, &buffer));
	}
	void SetStorage(GLsizei width, GLsizei height, GLsizei depth, GLint levels){
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
	void GiveData(TexData toGive){
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
private:
	TexType type;
	GLint	format;
	GLuint buffer;
	
};

#endif//TEXTURE_H