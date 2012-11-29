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
#define MAXDIFFUSE 3
#define MAXNORMAL  3
#define MAXSPEC	   3

class MarchingCubeAsteroid : public BasicGraphicsComponent{
	int numPairs;
	float time;
	Texture * tex3d;
	Texture * texs[MAXDIFFUSE + MAXNORMAL + MAXSPEC + 1]; // These will be used for normals + diffuse + Spec pairs and a blend texture to (proc generated????!)blend between pairs.
	Matrix<4, 4> transform;
	MarchingCubeAsteroid(VertexInfo * vInfo, int numVIs, int numVerts, Texture * tex3d);
	static UniformBufferObject<LookupTables>* tables;
	static void PopulateTables(); //Note, not in correct place.  Due to length of function this is at the bottom of the cpp.
	void LoadDiffuseNormalPairs(char* folderName, char* prefix);
	void LoadTexture(char* foldername, char* file, char* prefix);
	
public:
	static MarchingCubeAsteroid * Create(char* folderName, char* prefix);
	virtual void EntitySpecificShaderSetup();
	virtual void Render(int pass);
	void Update(float dt);

};

#endif//MARCHINGCUBEASTEROID_H