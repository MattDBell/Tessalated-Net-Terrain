#ifndef DRAWNENTITY_H
#define DRAWNENTITY_H

#include "Matrix.h"
#include <vector>
#include "ShaderProgram.h"
#include "VertexInfo.h"

class GraphicsComponent{

public: 	
	enum PrimitiveMode{
		PM_GL_LINES						= 0x0001,
		PM_GL_LINE_LOOP					= 0x0002,
		PM_GL_LINE_STRIP				= 0x0003,
		PM_GL_TRIANGLES					= 0x0004,
		PM_GL_TRIANGLE_STRIP			= 0x0005,
		PM_GL_TRIANGLE_FAN				= 0x0006,
		PM_GL_LINES_ADJACENCY			= 0x000A,
		PM_GL_LINE_STRIP_ADJACENCY		= 0x000B,
		PM_GL_TRIANGLES_ADJACENCY		= 0x000C,
		PM_GL_TRIANGLE_STRIP_ADJACENCY	= 0x000D,
		PM_GL_PATCHES					= 0xE
	};

private:
	GraphicsComponent();
protected:
	unsigned int	vao_ID;
	VertexInfo		* vInfos;
	const int		numVInfo;
	const int		numVerts;
	PrimitiveMode	primMode;
	
public:
	GraphicsComponent(VertexInfo* vInfos, int numVIs, int numVerts, PrimitiveMode pmode);
	virtual ~GraphicsComponent();
	virtual void Initialize();
	virtual void EntitySpecificShaderSetup();
	virtual void Render();

};
#endif //DRAWNENTITY_H