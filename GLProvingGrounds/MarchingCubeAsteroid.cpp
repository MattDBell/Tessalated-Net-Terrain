#include "MarchingCubeAsteroid.h"


MarchingCubeAsteroid::MarchingCubeAsteroid(VertexInfo * vInfo, int numVIs, int numVerts, Texture* tex3d)
	:BasicGraphicsComponent("MarchingCubeVertex.glsl", NULL, NULL, "MarchingCubeGeometry.glsl", "MarchingCubePixel.glsl", vInfo, numVIs, numVerts, GraphicsComponent::PM_GL_POINTS), tex3d(tex3d)
{
	
}

MarchingCubeAsteroid* MarchingCubeAsteroid::Create(){
	static bool firstRun = true;
	if(firstRun){
		PopulateTables();
		firstRun = false;
	}
	const int numVIs = 1;

	MVector<3> positions[32*32*32];
	for(int z = 0; z < 32; ++z){
		for(int y = 0; y < 32; ++y){
			for(int x = 0; x < 32; ++x){
				MVector<3> pos = {(float)x, (float)y, (float)z};
				positions[x + y * 32 + z *32 *32] = pos;
			}
		}
	}
	int numVerts = 32 * 32 * 32;
	//Generating histopyramids will be done in here so numVerts will change.  ALso the previous declared exhaustive
	//vertices will be thrown away.  But! For now, carry on.

	VertexInfo vIs[numVIs];
	vIs[0].Set("VertexPosition"	, true	, 32 *32 *32 * 3 * sizeof(GLfloat)	, positions,	VertexInfo::U_GL_STATIC_DRAW,
		1,	3,	VertexInfo::DT_GL_FLOAT,	false, 0, 0 );

	Texture * tex3d = new Texture(Texture::TT_GL_TEXTURE_3D, GL_R32F); //This will definitely change.

	tex3d->SetParamInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex3d->SetParamInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex3d->SetParamInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tex3d->SetParamInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tex3d->SetParamInt(GL_TEXTURE_WRAP_R, GL_REPEAT);

	Texture::TexData data;
	memset(&data, 0, sizeof(Texture::TexData));

	MVector<3> center = {16.0f, 16.0f, 16.0f};
	GLfloat d[32 * 32 * 32];
	for(int z = 0; z < 32; ++z){
		for(int y = 0; y < 32; ++y){
			for(int x = 0; x < 32; ++x){
				MVector<3> pos = {(float)x, (float)y, (float)z};
				d[x + y *32 + z * 32 * 32] = 5.0f - (pos-center).Length();
			}
		}
	}

	data.depth		= 32;
	data.width		= 32;
	data.height		= 32;

	data.level		= 0;
	data.format		= GL_R;
	data.type		= GL_FLOAT;
	data.data		=&d;

	tex3d->GiveData(data);
	
	return new MarchingCubeAsteroid(vIs, numVIs, numVerts, tex3d);


}

void MarchingCubeAsteroid::EntitySpecificShaderSetup()
{
	tex3d->MakeActive(0);
}

void MarchingCubeAsteroid::Render(int pass){
	BasicGraphicsComponent::Render(pass);
}

void MarchingCubeAsteroid::PopulateTables(){
	LookupTables table = {

	}
	tables = new UniformBufferObject<LookupTables>("LookupTablesMC");
}