#ifndef VERTEXINFO_H
#define VERTEXINFO_H
#include <memory>


struct VertexInfo{
	enum	DataType{
		DT_GL_BYTE							= 0x1400,
		DT_GL_UNSIGNED_BYTE					= 0x1401,
		DT_GL_SHORT							= 0x1402,
		DT_GL_UNSIGNED_SHORT				= 0x1403,
		DT_GL_INT							= 0x1404,
		DT_GL_UNSIGNED_INT					= 0x1405,
		DT_GL_FLOAT							= 0x1406,
		DT_GL_2_BYTES						= 0x1407,
		DT_GL_3_BYTES						= 0x1408,
		DT_GL_4_BYTES						= 0x1409,
		DT_GL_DOUBLE						= 0x140A,
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

	
	void Set(	char* name, bool isArrayBuffer, unsigned int dataBufferSize, void * dataBufferPtr, Usage usage,
		int  elementIndex, int sizePerVertexAttrib, DataType type, bool isNormalized, int stride, void* offset);
	

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
	
	void BindAttribLocation(int shaderBuffer);
	
	void Bind();
	void BufferData();
	void VertexAttribPointer();
	void Enable();
	void Initialize();
	~VertexInfo();
	
};
#endif//VERTEXINFO_H