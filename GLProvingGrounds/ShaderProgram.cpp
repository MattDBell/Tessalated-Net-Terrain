#include "ShaderProgram.h"
#include <fstream>
#include "Macros.h"


ShaderProgram::ShaderProgram(){
}

int ShaderProgram::Initialize( char * vertex, char * tessalationControl, char *tessaltionEvaluation, char * geometry, char * pixel){
	GLCALL(glUseProgram(0));
	for(int i = 0; i < TOTAL; ++i){
		shaderBuffers[i]	= 0;
		hasShader[i]		= false;
	}
	buffer = GLCALL(glCreateProgram());

	if(	LoadShaderPortion(vertex, GL_VERTEX_SHADER, VERTEX) &&
		LoadShaderPortion(tessalationControl, GL_TESS_CONTROL_SHADER, TESSALATION_CONTROL)&&
		LoadShaderPortion(tessaltionEvaluation, GL_TESS_EVALUATION_SHADER, TESSALATION_EVALUATION) &&
		hasShader[TESSALATION_CONTROL] == hasShader[TESSALATION_EVALUATION] &&
		LoadShaderPortion(geometry, GL_GEOMETRY_SHADER, GEOMETRY&&
		LoadShaderPortion(pixel, GL_FRAGMENT_SHADER, PIXEL) &&
		Link() != GL_FALSE))
	{
		return 1;
	}

	return 0;

}
int ShaderProgram::LoadShaderPortion(char * source, int ShaderType, int sBIndex){
	GLint result;
	if(source != 0){
		hasShader[sBIndex] = true;
		shaderBuffers[sBIndex] = GLCALL(glCreateShader(ShaderType));
		source = LoadSource(source);
		const GLchar * stupidFix = source;
		GLCALL(glShaderSource(shaderBuffers[sBIndex], 1, &stupidFix, NULL));
		GLCALL(glCompileShader(shaderBuffers[sBIndex]));
		GLCALL(glGetShaderiv(shaderBuffers[sBIndex], GL_COMPILE_STATUS, &result));
		if(result == GL_FALSE)
		{
			DebugShader(shaderBuffers[sBIndex]);
			return 0;
		}
		GLCALL(glAttachShader(buffer, shaderBuffers[sBIndex]));
		GLCALL(glDeleteShader(shaderBuffers[sBIndex]));
		free((void*)source);
	}
	return 1;
}
GLint ShaderProgram::Link(){
	GLint result;
	GLCALL(glLinkProgram(buffer));

	GLCALL(glGetProgramiv(buffer, GL_LINK_STATUS, &result));
	if(result == GL_FALSE){
		GLint length;
		char * log;
		GLCALL(glGetProgramiv(buffer, GL_INFO_LOG_LENGTH, &length));
		if(length ==0)
			return result;
		log = new GLchar[length];
		GLCALL(glGetProgramInfoLog(buffer, length, &result, log));
		delete[] log;
		GLCALL(glDeleteProgram(buffer));
		return GL_FALSE;
	}
	return GL_TRUE;
}
void ShaderProgram::DebugShader(GLuint buffer){
	GLint logLen;
	GLCALL(glGetShaderiv(buffer, GL_INFO_LOG_LENGTH, &logLen));
	if(logLen > 0){
		GLsizei strlen;
		GLchar * info = new GLchar[logLen];
		GLCALL(glGetShaderInfoLog(buffer, logLen, &strlen, info));
		delete [] info;
	}
}

char * ShaderProgram::LoadSource(char * fileName){
	FILE * fp;
	unsigned long f_size;
	char * ret;
	size_t sizeFile;
	fopen_s(&fp, fileName, "r");
	fseek(fp, 0, SEEK_END);
	f_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	sizeFile = sizeof(char) * f_size;
	ret = (char*)malloc(sizeFile + 1);
	memset(ret, 0, sizeFile + 1);
	fread(ret, sizeof(char), sizeFile, fp);
	for(size_t i = sizeFile-1; i >= 0; --i){
		if(ret[i] == '}'){			
			ret[i+1] = 0;
			break;
		}
	}
	fclose(fp);
	return ret;
}

void ShaderProgram::Update(float deltaTime){
	return;
}

void ShaderProgram::Bind(){
	GLCALL(glUseProgram(0));
	GLCALL(glUseProgram(buffer));
	current = this;
}


GLuint ShaderProgram::GetLocation(std::string name)
{
	if(Uniforms.find(name) == Uniforms.end())
		Uniforms[name] = glGetUniformLocation(buffer, name.c_str());
	return Uniforms[name];
}
void ShaderProgram::SetUniformFloat(std::string name, float value){
	GLuint location = GetLocation(name);
	GLCALL(glUniform1f(location, value));
}

void ShaderProgram::SetUniformMatrix(std::string name, Matrix<4, 4> value){
	GLuint location = GetLocation(name);
	GLCALL(glUniformMatrix4fv(location,  1, GL_FALSE, value.MatrixPointer()));
}
void ShaderProgram::SetUniformVector(std::string name, MVector<3> value){
	GLuint location = GetLocation(name);
	GLCALL(glUniform3fv(location, 1, value.GetValues()));
}