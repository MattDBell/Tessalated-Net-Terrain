#ifndef DRAWNENTITY_H
#define DRAWNENTITY_H

#include "Matrix.h"
#include <vector>
#include "ShaderProgram.h"
#include "VertexInfo.h"

class GraphicsComponent{

public: 	
	enum PrimitiveMode{
		PM_GL_POINTS					= GL_POINTS,
		PM_GL_LINES						= GL_LINES,
		PM_GL_LINE_LOOP					= GL_LINE_LOOP,
		PM_GL_LINE_STRIP				= GL_LINE_STRIP,
		PM_GL_TRIANGLES					= GL_TRIANGLES,
		PM_GL_TRIANGLE_STRIP			= GL_TRIANGLE_STRIP,
		PM_GL_TRIANGLE_FAN				= GL_TRIANGLE_FAN,
		PM_GL_LINES_ADJACENCY			= GL_LINES_ADJACENCY,
		PM_GL_LINE_STRIP_ADJACENCY		= GL_LINE_STRIP_ADJACENCY,
		PM_GL_TRIANGLES_ADJACENCY		= GL_TRIANGLES_ADJACENCY,
		PM_GL_TRIANGLE_STRIP_ADJACENCY	= GL_TRIANGLE_STRIP_ADJACENCY,
		PM_GL_PATCHES					= GL_PATCHES
	};
	
private:
	GraphicsComponent();
	GraphicsComponent& operator=(GraphicsComponent& rhs);
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
	virtual void Render(int pass);
};
#endif //DRAWNENTITY_H