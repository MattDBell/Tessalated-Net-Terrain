#ifndef MARCHINGCUBEASTEROID_H
#define MARCHINGCUBEASTEROID_H

#include "BasicGraphicsComponent.h"
#include "Texture.h"
#include "UniformBufferObject.h"
#include <cstdint>
//Plain Old Data Type for UBO
struct LookupTables{
	uint32_t edgetable	[64  * 4];	
	uint32_t triTable	[256 * 4]; //See MarchingCubeGeometry which stores integers in ivec4s
};

class MarchingCubeAsteroid : public BasicGraphicsComponent{
	
	Texture * tex3d;
	Matrix<4, 4> transform;
	MarchingCubeAsteroid(VertexInfo * vInfo, int numVIs, int numVerts, Texture * tex3d);
	static UniformBufferObject<LookupTables>* tables;
	static void PopulateTables();

public:
	static MarchingCubeAsteroid * Create();
	virtual void EntitySpecificShaderSetup();
	virtual void Render(int pass);

};

#endif//MARCHINGCUBEASTEROID_H