#ifndef MARCHINGCUBEASTEROID_H
#define MARCHINGCUBEASTEROID_H

#include "BasicGraphicsComponent.h"
#include "Texture.h"

class MarchingCubeAsteroid : public BasicGraphicsComponent{
	
	Texture * tex3d;

	MarchingCubeAsteroid();
public:

	static MarchingCubeAsteroid * Create();
	virtual void EntitySpecificShaderSetup();
	virtual void Render(int pass);

}:

#endif//MARCHINGCUBEASTEROID_H