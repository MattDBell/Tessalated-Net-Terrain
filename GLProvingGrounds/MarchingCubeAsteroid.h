#ifndef MARCHINGCUBEASTEROID_H
#define MARCHINGCUBEASTEROID_H

#include "BasicGraphicsComponent.h"
#include "Texture.h"
#include "UniformBufferObject.h"
//Plain Old Data Type for UBO
struct LookupTables{
	int edgetable	[64  * 4];	
	int triTable	[256 * 4]; //See MarchingCubeGeometry which stores integers in ivec4s
};

class MarchingCubeAsteroid : public BasicGraphicsComponent{
	
	Texture * tex3d;

	MarchingCubeAsteroid(VertexInfo * vInfo, int numVIs, int numVerts, Texture * tex3d);
	static UniformBufferObject<LookupTables>* tables;
	static void PopulateTables();

public:
	static MarchingCubeAsteroid * Create();
	virtual void EntitySpecificShaderSetup();
	virtual void Render(int pass);

};

#endif//MARCHINGCUBEASTEROID_H