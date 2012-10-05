#ifndef VERTEXINFO_H
#define VERTEXINFO_H
#include "Macros.h"
#include <memory>


struct VertexInfo{
	enum	DataType{
		DT_GL_UNSIGNED_INT					= 0x1405, 
		DT_GL_FLOAT							= 0x1406,
		DT_GL_COUNTER_TYPE_AMD				= 0x8BC0,
		DT_GL_COUNTER_RANGE_AMD				= 0x8BC1,
		DT_GL_UNSIGNED_INT64_AMD			= 0x8BC2,
		DT_GL_PERCENTAGE_AMD				= 0x8BC3,
		DT_GL_PERFMON_RESULT_AVAILABLE_AMD	= 0x8BC4,
		DT_GL_PERFMON_RESULT_SIZE_AMD		= 0x8BC5,
		DT_GL_PERFMON_RESULT_AMD			= 0x8BC6 };

	enum	Usage{
		U_GL_STREAM_DRAW		= 0x88E0,
		U_GL_STREAM_READ		= 0x88E1,
		U_GL_STREAM_COPY		= 0x88E2,
		U_GL_STATIC_DRAW		= 0x88E4,
		U_GL_STATIC_READ		= 0x88E5,
		U_GL_STATIC_COPY		= 0x88E6,
		U_GL_DYNAMIC_DRAW		= 0x88E8,
		U_GL_DYNAMIC_READ		= 0x88E9,
		U_GL_DYNAMIC_COPY		= 0x88EA
	};

	char*			name;

	//Bind Buffer Variables
	bool			bIsArrayBuffer; //Used in Buffer Data as well
	unsigned int	iBuffer;

	//Buffer Data Variables
	unsigned int			dataBufferSize;
	std::shared_ptr<void>	dataBuffer;
	Usage					usage;

	//Vertex Attrib Pointer Variables
	int				iElementIndex;
	int				sizePerVertexAttrib;
	DataType		type;
	bool			bIsNormalized;
	int				stride;
	void*			pointer;
	
	void BindAttribLocation(int shaderBuffer){
		if(!bIsArrayBuffer)
			return;
		GLCALL(glBindAttribLocation(shaderBuffer, iElementIndex, name));
	}
	void Bind(){
		GLCALL(glBindBuffer(bIsArrayBuffer ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, iBuffer));
	}
	void BufferData(){
		GLCALL(glBufferData(bIsArrayBuffer ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER,
			dataBufferSize, dataBuffer.get(), usage));
	}
	void VertexAttribPointer(){
		if(!bIsArrayBuffer)
			return;
		GLCALL(glVertexAttribPointer(iElementIndex, sizePerVertexAttrib, type, bIsNormalized ? GL_TRUE : GL_FALSE, stride, pointer));
	}
	void Initialize(){
		Bind();
		BufferData();
		VertexAttribPointer();
	}

	void Enable(){
		if(!bIsArrayBuffer)
			return;
		GLCALL(glEnableVertexAttribArray(iElementIndex));
	}
	~VertexInfo(){
		GLCALL(glDeleteBuffers(1, &iBuffer));
	}
};
#endif//VERTEXINFO_H