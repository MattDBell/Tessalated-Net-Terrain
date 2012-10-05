#ifndef  UNIFORMBUFFEROBJECT_H
#define  UNIFORMBUFFEROBJECT_H
#include "Macros.h"
#include <vector>

struct UBOinterface{
	virtual void BindToShader(unsigned int programBuffer);
};

struct UBOStatics{
private:
	static UBOStatics * instance;
public:
	UBOStatics* Get(){
		if(instance == NULL)
			UBOStatics::instance = new UBOStatics();
		return instance;
	}
	int index;
	std::vector<UBOinterface *> buffers;
	void BindAll(unsigned int program){
		for(auto it = buffers.begin(); it != buffers.end(); it++)
			(*it)->BindToShader(program);
	}
};

template <class dataStructure>
struct UniformBufferObject : public UBOinterface{
	static unsigned int currIndex;
	unsigned int		uBOBindingIndex;
	unsigned int		uboIndex;
	char *				name;
	UniformBufferObject(){
		
		currIndex = UBOStatics::Get()->Index++;
		UBOStatics::Get()->buffers.push_back((void *)this);
		Initialize();
	}
	~UniformBufferObject(){
		bool found = false;
		for(auto it = allBuffers.begin(); it != allBuffers.end(); it++){
			if((*it)  == this){
				(*it) = allBuffers[allBuffers.size -1];
				found = true;
				break;
			}
		}
		if(found)
			allBuffers.pop_back;
		if(name != NULL){
			delete [] name;
			name = NULL;
		}
		GLCALL(glDeleteBuffers(1, &uboIndex));
	}
	void Initialize(){
		GLCALL(glGenBuffers(1, &uboIndex));
		GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uboIndex));
		GLCALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(dataStructure), NULL, GL_STREAM_DRAW));
		GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		GLCALL(glBindBufferRange(GL_UNIFORM_BUFFER, uBOBindingIndex, uboIndex, 0, sizeof(dataStructure)));
	}

	void BindToShader(unsigned int programBuffer){
		unsigned int uniformBlockIndex = GLCALL(glGetUniformBlockIndex(programBuffer, name));
		if(uniformBlockIndex == GL_INVALID_INDEX)
			return;
		GLCALL(glUniformBlockBinding(programBuffer, uniformBlockIndex, uBOBindingIndex));
	}
	void Update(dataStructure* data){
		GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uboIndex));
		GLCALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(dataStructure), data, GL_STREAM_DRAW));
	}
};
#endif //UNIFORMBUFFEROBJECT_H
