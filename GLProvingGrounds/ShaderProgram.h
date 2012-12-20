#ifndef SHADERPROGRAM
#define SHADERPROGRAM

#include <GL/glew.h>
#include <string>
#include <map>
#include "Matrix.h"

class DrawnEntity;

class  ShaderProgram{
	enum {VERTEX, TESSALATION_CONTROL, TESSALATION_EVALUATION, GEOMETRY, PIXEL, TOTAL};
	static ShaderProgram*			current;
	unsigned int					buffer;
	unsigned int					shaderBuffers[TOTAL];
	bool							hasShader[TOTAL];
	std::map<std::string, GLuint>	Uniforms;

	char*			LoadSource(char * fileName);
	GLuint			GetLocation(std::string name);
	void			DebugShader(GLuint buffer);
	int				LoadShaderPortion(char * source, int ShaderType, int buffer);
public:
	friend class GraphicsComponent;
	static ShaderProgram*	GetCurrent() { return current;}
	
	ShaderProgram();
	unsigned int	GetBuffer(){ return buffer;}
	virtual void	SetUniformFloat(std::string name, float value);
	virtual void	SetUniformMatrix(std::string name, Matrix<4, 4> value);
	virtual void	SetUniformVector(std::string name, MVector<3> value);
	virtual void	SetUniformInt(std::string name, int value);
	virtual void	Update(float deltaTime);
	virtual void	Bind();
	virtual GLint	Link();
	virtual int		Initialize(char * vertex, char * tessalationControl, char *tessaltionEvaluation, char * geometry, char * pixel);
};

#endif