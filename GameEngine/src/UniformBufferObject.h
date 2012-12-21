#ifndef  UNIFORMBUFFEROBJECT_H
#define  UNIFORMBUFFEROBJECT_H
#include "Macros.h"
#include "RedirectedGlew.h"
#include <vector>
#include <GL/glew.h>
#define OFFSETOF(CLASS, MEMBER)  ((char*)&((( CLASS* )0)->MEMBER) - (char*)0 )

struct UBOinterface{
	virtual void BindToShader(unsigned int programBuffer) = 0;
};

struct UBOStatics{
private:
	static UBOStatics * instance;
	UBOStatics()
		:index(1)
	{}
public:
	static UBOStatics* Get();
	int index;
	std::vector<UBOinterface *> buffers;
	std::vector<UBOinterface *>& GetBuffers();
	void BindAll(unsigned int program);
};

template <class dataStructure>
struct UniformBufferObject : public UBOinterface{
	typedef dataStructure data;
	unsigned int		uBOBindingIndex;
	unsigned int		uboIndex;
	char *				name;
	UniformBufferObject(char * name){
		int size = strlen(name) + 1;
		this->name = (char*)malloc(size);
		strcpy_s(this->name, size, name);
		uBOBindingIndex = UBOStatics::Get()->index++;
		UBOStatics::Get()->buffers.push_back(this);
		Initialize();
	}
	~UniformBufferObject(){
		bool found = false;
		std::vector<UBOinterface *> allBuffers = UBOStatics::Get()->GetBuffers();
		for(auto it = allBuffers.begin(); it != allBuffers.end(); it++){
			if((*it)  == this){
				(*it) = allBuffers[allBuffers.size() -1];
				found = true;
				break;
			}
		}
		
		if(found)
			allBuffers.pop_back();
		if(name != NULL){
			free(name);
		}
		rglDeleteBuffers(1, &uboIndex);
	}
	void Initialize(){
		int i = OFFSETOF(UniformBufferObject<int>, uboIndex);
		printf("%d", i);
		rglGenBuffers(1, &uboIndex);
		rglBindBuffer(GL_UNIFORM_BUFFER, uboIndex);
		rglBufferData(GL_UNIFORM_BUFFER, sizeof(dataStructure), NULL, GL_STREAM_DRAW);
		rglBindBuffer(GL_UNIFORM_BUFFER, 0);
		rglBindBufferRange(GL_UNIFORM_BUFFER, uBOBindingIndex, uboIndex, 0, sizeof(dataStructure));
	}

	void BindToShader(unsigned int programBuffer){
		unsigned int uniformBlockIndex = GLCALL(glGetUniformBlockIndex(programBuffer, name));
		if(uniformBlockIndex == GL_INVALID_INDEX)
			return;
		GLCALL(glUniformBlockBinding(programBuffer, uniformBlockIndex, uBOBindingIndex));
	}
	void SubUpdate(GLintptr offset, GLsizeiptr size, void* data){
		GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uboIndex));
		GLCALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
	}
	void Update(dataStructure* data){
		GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uboIndex));
		GLCALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(dataStructure), data, GL_STREAM_DRAW));
	}
};
#endif //UNIFORMBUFFEROBJECT_H
