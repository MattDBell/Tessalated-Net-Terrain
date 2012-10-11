#ifndef BASICGRAPHICSCOMPONENT_H
#define BASICGRAPHICSCOMPONENT_H

#include "Matrix.h"
#include "GraphicsComponent.h"
#include <vector>
#include "ShaderProgram.h"
#include "VertexInfo.h"

class BasicGraphicsComponent : public GraphicsComponent{
private:
	BasicGraphicsComponent();
protected:
	
	ShaderProgram	* shader;

	char			*vertex, *tessalationControl, *tessalationEvaluation, *geometry, *pixel;
	
public:
	BasicGraphicsComponent(char * vertex, char * tessalationControl, char * tessalationEvaluation,  char * geometry, char * pixel,  VertexInfo* vInfos, int numVIs, int numVerts, PrimitiveMode pmode);
	virtual ~BasicGraphicsComponent();
	virtual void Initialize();
	virtual void EntitySpecificShaderSetup();
	virtual void Render();

};
#endif //BASICGRAPHICSCOMPONENT_H