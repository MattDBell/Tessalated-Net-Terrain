#ifndef TEXTURE_H
#define TEXTURE_H
#include "Macros.h"

class Texture{
public:
	enum TexType{
		TT_GL_TEXTURE_1D					= 0x0DE0,
		TT_GL_TEXTURE_2D					= 0x0DE1,
		TT_GL_TEXTURE_3D					= 0x806F,
		TT_GL_TEXTURE_RECTANGLE				= 0x84F5,
		TT_GL_TEXTURE_BUFFER				= 0x8C2A,
		TT_GL_TEXTURE_CUBE_MAP				= 0x8513,
		TT_GL_TEXTURE_1D_ARRAY				= 0x8C18,
		TT_GL_TEXTURE_2D_ARRAY				= 0x8C1A,
		TT_GL_TEXTURE_CUBE_MAP_ARRAY		= 0x9009,
		TT_GL_TEXTURE_2D_MULTISAMPLE		= 0x9100,
		TT_GL_TEXTURE_2D_MULTISAMPLE_ARRAY	= 0x9102
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

	Texture(TexType t, GLint format);
	void SetStorage(GLsizei width, GLsizei height, GLsizei depth, GLint levels);
	void GiveData(TexData toGive);
	
	void SetParamFloat			(GLenum	pName, GLfloat		param);
	void SetParamInt			(GLenum	pName, GLint		param);	
	void SetParamFloatVector	(GLenum	pName, GLfloat*		param);
	void SetParamFloatVector	(GLenum	pName, GLint*		param);
	void SetParamIntVector		(GLenum	pName, GLint*		param);
	void SetParamIntVector		(GLenum	pName, GLuint*		param);

	void GenerateMipMaps();
	void MakeActive(unsigned int slot);
private:
	TexType type;
	GLint	format;
	GLuint buffer;
	int boundTo;
	
};

#endif//TEXTURE_H