#ifndef SHADERPROGRAM
#define SHADERPROGRAM

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
	std::map<std::string, unsigned int>	Uniforms;

	char*			LoadSource(char * fileName);
	unsigned int	GetLocation(std::string name);
	void			DebugShader(unsigned int buffer);
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
	virtual int		Link();
	virtual int		Initialize(char * vertex, char * tessalationControl, char *tessaltionEvaluation, char * geometry, char * pixel);
};

#endif