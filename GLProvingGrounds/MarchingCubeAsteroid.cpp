#include <Windows.h> //folder shtuff
#include "MarchingCubeAsteroid.h"
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <ctime>
#include "Input.h"


#define PACKFOUR(a, b, c, d) ((a << 24) | ((b & 0xff) << 16) | ((c & 0xff) << 8) | (d & 0xff))
#define PACKSIXTEEN(a, b, c, d, a2, b2, c2, d2, a3, b3, c3, d3, a4, b4, c4, d4) PACKFOUR(a, b, c, d), PACKFOUR(a2, b2, c2, d2), PACKFOUR(a3, b3, c3, d3), PACKFOUR(a4, b4, c4, d4)
const float PI = 3.1415926f;
UniformBufferObject<LookupTables>* MarchingCubeAsteroid::tables = 0;

MarchingCubeAsteroid::MarchingCubeAsteroid(VertexInfo * vInfo, int numVIs, int numElements, Texture* tex3d)
	:BasicGraphicsComponent("MarchingCubeVertex.glsl", NULL, NULL, "MarchingCubeGeometry.glsl", "MarchingCubePixel.glsl", vInfo, numVIs, 
	numElements, GraphicsComponent::PM_GL_POINTS), tex3d(tex3d), numPairs(0), currtime(0), updateState(SPHERE)
{
	MVector<4> t = {-16, -16, -16, 1};
	transform = Translate(t);
}

MarchingCubeAsteroid* MarchingCubeAsteroid::Create(char * folderName, char* prefix){

	//Static initialization
	static bool firstRun = true;
	if(firstRun){
		PopulateTables();
		firstRun = false;
	}
	const int numVIs = 1;
	
	//Create raw position data
	MVector<3>* positions = new MVector<3>[31*31*31];
	for(int z = 0; z < 31; ++z){
		for(int y = 0; y < 31; ++y){
			for(int x = 0; x < 31; ++x){
				MVector<3> pos = {(float)x, (float)y, (float)z};
				positions[x + y * 31 + z *31 *31] = pos;
			}
		}
	}
	int numElements = 31 * 31 * 31;
	//Generating histopyramids will be done in here so numVerts will change.  ALso the previous declared exhaustive
	//vertices will be thrown away.  But! For now, carry on.

	VertexInfo *vIs = new VertexInfo[numVIs];
	vIs[0].Set("VertexPosition"	, true	, 31 *31 *31 * 3 * sizeof(GLfloat)	, positions,	VertexInfo::U_GL_STATIC_DRAW,
		1,	3,	VertexInfo::DT_GL_FLOAT,	false, 0, 0 );

	//Creation of Density field

	Texture * tex3d = new Texture(Texture::TT_GL_TEXTURE_3D, GL_R32F); //This will definitely change.

	tex3d->SetParamInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex3d->SetParamInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex3d->SetParamInt(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tex3d->SetParamInt(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tex3d->SetParamInt(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	Texture::TexData data;
	memset(&data, 0, sizeof(Texture::TexData));

	GLfloat d[32 * 32 * 32];
	for(int z = 0; z < 32; ++z){
		for(int y = 0; y < 32; ++y){
			for(int x = 0; x < 32; ++x){
				MVector<3> center = {16.0f, 16.0f, 16.0f}; MVector<3> pos = {(float)x, (float)y, (float)z}; d[x + y *32 + z * 32 * 32] = 10.0f - (pos-center).Length();// + (rand() % 100) / 100.0f ;
				//d[x + y *32 + z * 32 * 32] =(float)((rand() % 10) -7);
				//d[x + y *32 + z * 32 * 32] = (float) y -10;
				//d[x + y *32 + z * 32 * 32] = (float) z -10;
			}
		}
	}

	data.depth		= 32;
	data.width		= 32;
	data.height		= 32;

	data.level		= 0;
	data.format		= GL_RED;
	data.type		= GL_FLOAT;
	data.data		=&d;

	tex3d->GiveData(data);

	
	MarchingCubeAsteroid* newMCA = new MarchingCubeAsteroid(vIs, numVIs, numElements, tex3d);
	newMCA->LoadDiffuseNormalPairs(folderName,prefix);
	return newMCA;
}
#pragma warning(push)
#pragma warning(disable: 4996)
void MarchingCubeAsteroid::LoadDiffuseNormalPairs(char * folderName, char* prefix)
{
	assert(folderName && prefix);

	WCHAR path[256];
	WCHAR fname[256];
	WCHAR pre[32];
	int folderNameLength = strlen(folderName) + 1;
	assert(folderNameLength < 256);
	mbstowcs(fname, folderName, folderNameLength);

	int preLength = strlen(prefix) + 1;
	assert(preLength < 32);
	mbstowcs(pre, prefix, preLength);

	memcpy(&fname[folderNameLength-1], pre, preLength * sizeof(WCHAR));

	DWORD pathLength =GetCurrentDirectory(244, path);
	path[pathLength++] = '\\';
	assert(pathLength + folderNameLength < 256);
	memcpy(&path[pathLength], fname, (folderNameLength + preLength -1 ) * sizeof(WCHAR));
	path[pathLength + folderNameLength + preLength -2 ] = '*'; //two null characters are included in lengths
	path[pathLength + folderNameLength + preLength -1 ] = '\0';

	WIN32_FIND_DATA foundFile;
	HANDLE search = FindFirstFile(path, &foundFile);
	for(size_t i = 0; i < MAXDIFFUSE + MAXNORMAL + MAXSPEC ; ++i)
	{
		char fileName[260];
		if( search == INVALID_HANDLE_VALUE || !wcstombs(fileName, foundFile.cFileName, 260))
			break;
		LoadTexture(folderName, fileName, prefix);
		if(!FindNextFile(search, &foundFile)) break;
	}
}

#pragma pack(push, 1)
struct TGAHeader
{
	__int8		idLen;
	__int8		colMap;
	__int8		imgType;
	__int16		colMapFirstEntryOff;
	__int16		colMapLength;
	__int8		colMapEntrySize;
	__int16		xOrig;
	__int16		yOrig;
	__int16		width;
	__int16		height;
	__int8		bitsPerPixel;
	__int8		imgDesc;
};
GLint GetFormat(int bpp, int imgType)
{
	switch (bpp)
	{
	case 24:
		switch(imgType)
		{
		case 2:
			return GL_RGB8;
		case 10:
			return GL_RGB8;
		}

	}
	return 0;
}
void MarchingCubeAsteroid::LoadTexture(char* foldername, char* file, char* prefix)
{
	int pLen = strlen(prefix);
	int number = (file[pLen + 1]) - '0';
	int type = (file[pLen + 3] == 'c') ? 0 : file[pLen + 3] == 'n'? 1 : file[pLen + 3] == 's' ? 2 : 0xff;
	
	char relFile[256];
	int folderSize = strlen(foldername);
	memcpy(relFile, foldername, folderSize);
	memcpy(&relFile[folderSize], file, 255 - folderSize);

	//Ensure tga 2.0
	char endOfFile[27];
	FILE * f = fopen(relFile, "r");
	fseek(f, 0, SEEK_END);
	long offEnd = ftell(f);
	rewind(f);
	fseek(f, offEnd-26, SEEK_SET);
	fread(endOfFile, 1, 26, f);
	endOfFile[26] = '\0';
	if(strcmp(&endOfFile[8], "TRUEVISION-XFILE."))
		return; //My very simple reader does not deal with old TGAs, who do you think I am???
	
	//read header
	rewind(f);
	TGAHeader head;
	fread(&head, 1, sizeof(head), f);

	bool top = ((head.imgDesc >> 5) & 1) == 1;
	bool right = ((head.imgDesc >>4 & 1) << 4) == 1; 
	__int8 alphaDepth = head.imgDesc & 0xf;
	(alphaDepth);

	char * imgID;
	if(head.idLen)
	{
		imgID = new char[head.idLen];
		fread(imgID, 1, head.idLen, f);
	}
	char * colMap;
	if(head.colMapLength) 
	{
		colMap = new char[head.colMapLength * (head.colMapEntrySize+7) /8 ];
		fread(colMap, 1, head.colMapLength * head.colMapEntrySize, f);
	}
	GLint gl_F =  GetFormat(head.bitsPerPixel, head.imgType);
	assert(gl_F);
	
	
	

	__int8 * data = new __int8[head.width * head.height * 3];
	int curr = 0;
	int y = 0, x = 0;
	switch(head.imgType)
	{
	case 2:
		{
			__int8 * fInfo = new __int8[head.width * head.height * 3];
			fread(&fInfo[0], 3, head.width * head.height, f);
			for(y = 0; y < head.height; ++y)
			{
				for(x = 0; x < head.width; ++x)
				{
					
					
					int actX = right? (head.width -1 - x) : x;
					int actY = top ? (head.height -1 - y) : y;
					data[(actX )*3 + 2 + actY * head.width * 3]	= fInfo[x * 3 + y * head.width * 3];
					data[(actX )*3 + 1 + actY * head.width * 3]	= fInfo[x * 3 + y * head.width * 3 + 1];
					data[(actX )*3 + actY * head.width * 3]		= fInfo[x * 3 + y * head.width * 3 + 2];
				}
		
			}
			free(fInfo);
		}
		break;
	case 10:
		{
			int size = offEnd - ftell(f);
			__int8* fInfo = new __int8[size];
			fread(fInfo, 1, size, f);
			int place = 0;
			while( curr < head.width * head.height)
			{
				__int8 repCount = fInfo[place++];
				bool raw = (repCount & 0x80) == 0;
				int count = (repCount & 0x7f) + 1;
				if(raw)
				{
					for(int i = 0; i < count; ++i)
					{
						int x = curr % head.width;
						int y = curr / head.width;
						int actX = right? (head.width -1 - x) : x;
						int actY = top ? (head.height -1 - y) : y;
						data[(actX )*3 + 2 + actY * head.width * 3] = fInfo[place++];
						data[(actX )*3 + 1 + actY * head.width * 3] = fInfo[place++];
						data[(actX )*3 + actY * head.width * 3]		= fInfo[place++];
						++curr;
					}
				}
				else
				{
					__int8 value[3] = {fInfo[place +2], fInfo[place + 1], fInfo[place]};
					place += 3;
					for(int i = 0; i < count; ++i)
					{
						int x = curr % head.width;
						int y = curr / head.width;
						int actX = right? (head.width -1 - x) : x;
						int actY = top ? (head.height -1 - y) : y;
						memcpy(&data[actX * 3 + actY * head.width * 3], value, 12);
						++curr;
					}
				}
			}
			free(fInfo);
		}
		break;
	default:
		assert(0);
	}

	int offset = type == 0? 0 : type ==1 ? MAXDIFFUSE : type == 2 ? MAXDIFFUSE + MAXNORMAL : 0xff;
	if(offset == 0xff) return;
	Texture * tex = new Texture(Texture::TT_GL_TEXTURE_2D, gl_F);

	Texture::TexData td;
	memset(&td, 0, sizeof(td));
	td.data = data;
	td.format = GL_RGB;
	td.height = head.height;
	td.width = head.width;
	td.level = 0;
	td.type = GL_UNSIGNED_BYTE;

	tex->GiveData(td);
	tex->SetParamInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex->SetParamInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex->SetParamInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tex->SetParamInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tex->SetParamInt(GL_TEXTURE_WRAP_R, GL_REPEAT);


	texs[(number-1) + offset] = tex;
	numPairs |= 1 << ((number-1) + offset);
	fclose(f);
}
#pragma pack(pop)
#pragma warning(pop)
void MarchingCubeAsteroid::EntitySpecificShaderSetup()
{
	shader->SetUniformMatrix("model", transform);
	shader->SetUniformInt("TexMap", numPairs);
	shader->SetUniformFloat("time", currtime);
	shader->SetUniformInt("state", (int)updateState);
	for(int i = 0; i < MAXDIFFUSE + MAXNORMAL + MAXSPEC; ++i)
	{
		if(((numPairs >> i) & 1) == 1)
			texs[i]->MakeActive(i + 2); //first slot is for 3d tex
	}
	tex3d->MakeActive(1);
}

void MarchingCubeAsteroid::Render(int pass){
	BasicGraphicsComponent::Render(pass);
}

void MarchingCubeAsteroid::Update(float dt){
	//return;
	static bool shrink = false;
	dt = shrink? -dt : dt;
	currtime += dt;
	srand(randSeed);
	static GLfloat d[32 * 32 * 32];
	for(int z = 0; z < 32; ++z){
		for(int y = 0; y < 32; ++y){
			for(int x = 0; x < 32; ++x){
				switch(updateState)
				{
				case SINE_WAVE_THING:
				default:
					{
						if(currtime > 10.0f) 
						{
							currtime = 10;
							shrink = true;
						} else if (currtime < 0)
						{
							currtime = 0;
							shrink = false;
						}
						MVector<3> center = {16.0f, 16.0f, 16.0f};
						MVector<3> pos = {(float)x, (float)y, (float)z};
						d[x + y *32 + z * 32 * 32] = 3 + currtime - (pos-center).Length() + cos(x + currtime) + (rand() % 100) / 100.0f ;
					}
					break;
				case MESS:
					{
						d[x + y *32 + z * 32 * 32] =(float)((rand() % 10) -7);
					}
					break;
				case FACETEDSPHERE:
					{
						MVector<3> center = {16.0f, 16.0f, 16.0f};
						MVector<3> pos = {(float)x, (float)y, (float)z};
						d[x + y *32 + z * 32 * 32] =  10 - (pos-center).Length() + (rand() % 100) / 100.0f;
					}
					break;
				case SPHERE:
					{
						MVector<3> center = {16.0f, 16.0f, 16.0f};
						MVector<3> pos = {(float)x, (float)y, (float)z};
						d[x + y *32 + z * 32 * 32] =  10 - (pos-center).Length();
					}
					break;
				case MALLEABLE:
					{
						static bool lastState = false;
						int xPos, yPos;
						bool currState = Input::Get()->GetLastMousePos(xPos, yPos);
						if(lastState == true && lastState != currState)
						{
							//deproject x, y
							//cast to start of ME!
							//Step through until I hit something
							//sphere cast(ish)
						}
						lastState = currState;
					}
					break;
				case TERRAIN:
					//do nothing
					break;
				}
			}
		}
	}
	Texture::TexData data;
	memset(&data, 0, sizeof(data));
	data.depth		= 32;
	data.width		= 32;
	data.height		= 32;
	
	data.level		= 0;
	data.format		= GL_RED;
	data.type		= GL_FLOAT;
	data.data		=&d;
	
	tex3d->GiveData(data);
}
void MarchingCubeAsteroid::SetState(States to)
{
	if(to == updateState) return;
	if((unsigned int) to < TOTAL)
		updateState = to;
	else
		updateState = SPHERE;
	srand( (unsigned int) time(NULL) ) ;
	randSeed = rand();
}

void MarchingCubeAsteroid::PopulateTables(){
	LookupTables table = { /*edge Table [64] ivec4*/
		{0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
		0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
		0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
		0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
		0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
		0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
		0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
		0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
		0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
		0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
		0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
		0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
		0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
		0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
		0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
		0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
		0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
		0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
		0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
		0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
		0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
		0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
		0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
		0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
		0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
		0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
		0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
		0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
		0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
		0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
		0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
		0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   },
		{
			PACKSIXTEEN(255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 3U, 255U,  255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 1U, 9U, 255U,  255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 8U, 3U, 9U, 8U, 1,	255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 3U, 1U, 2U, 10U,  255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 2U, 10U, 0U, 2U, 9U,  255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 8U, 3U, 2U, 10U, 8U, 10U, 9U, 8U,  255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 11U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 11U, 2U, 8U, 11U, 0U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 9U, 0U, 2U, 3U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 11U, 2U, 1U, 9U, 11U, 9U, 8U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 10U, 1U, 11U, 10U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 10U, 1U, 0U, 8U, 10U, 8U, 11U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 9U, 0U, 3U, 11U, 9U, 11U, 10U, 9U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 8U, 10U, 10U, 8U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 7U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 3U, 0U, 7U, 3U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 1U, 9U, 8U, 4U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 1U, 9U, 4U, 7U, 1U, 7U, 3U, 1U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 10U, 8U, 4U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 4U, 7U, 3U, 0U, 4U, 1U, 2U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 2U, 10U, 9U, 0U, 2U, 8U, 4U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 10U, 9U, 2U, 9U, 7U, 2U, 7U, 3U, 7U, 9U, 4U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 4U, 7U, 3U, 11U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(11U, 4U, 7U, 11U, 2U, 4U, 2U, 0U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 0U, 1U, 8U, 4U, 7U, 2U, 3U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 7U, 11U, 9U, 4U, 11U, 9U, 11U, 2U, 9U, 2U, 1U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 10U, 1U, 3U, 11U, 10U, 7U, 8U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 11U, 10U, 1U, 4U, 11U, 1U, 0U, 4U, 7U, 11U, 4U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 7U, 8U, 9U, 0U, 11U, 9U, 11U, 10U, 11U, 0U, 3U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 7U, 11U, 4U, 11U, 9U, 9U, 11U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 5U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 5U, 4U, 0U, 8U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 5U, 4U, 1U, 5U, 0U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 5U, 4U, 8U, 3U, 5U, 3U, 1U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 10U, 9U, 5U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 0U, 8U, 1U, 2U, 10U, 4U, 9U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 2U, 10U, 5U, 4U, 2U, 4U, 0U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 10U, 5U, 3U, 2U, 5U, 3U, 5U, 4U, 3U, 4U, 8U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 5U, 4U, 2U, 3U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 11U, 2U, 0U, 8U, 11U, 4U, 9U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 5U, 4U, 0U, 1U, 5U, 2U, 3U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 1U, 5U, 2U, 5U, 8U, 2U, 8U, 11U, 4U, 8U, 5U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 3U, 11U, 10U, 1U, 3U, 9U, 5U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 9U, 5U, 0U, 8U, 1U, 8U, 10U, 1U, 8U, 11U, 10U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 4U, 0U, 5U, 0U, 11U, 5U, 11U, 10U, 11U, 0U, 3U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 4U, 8U, 5U, 8U, 10U, 10U, 8U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 7U, 8U, 5U, 7U, 9U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 3U, 0U, 9U, 5U, 3U, 5U, 7U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 7U, 8U, 0U, 1U, 7U, 1U, 5U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 5U, 3U, 3U, 5U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 7U, 8U, 9U, 5U, 7U, 10U, 1U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 1U, 2U, 9U, 5U, 0U, 5U, 3U, 0U, 5U, 7U, 3U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 0U, 2U, 8U, 2U, 5U, 8U, 5U, 7U, 10U, 5U, 2U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 10U, 5U, 2U, 5U, 3U, 3U, 5U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 9U, 5U, 7U, 8U, 9U, 3U, 11U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 5U, 7U, 9U, 7U, 2U, 9U, 2U, 0U, 2U, 7U, 11U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 3U, 11U, 0U, 1U, 8U, 1U, 7U, 8U, 1U, 5U, 7U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(11U, 2U, 1U, 11U, 1U, 7U, 7U, 1U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 5U, 8U, 8U, 5U, 7U, 10U, 1U, 3U, 10U, 3U, 11U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 7U, 0U, 5U, 0U, 9U, 7U, 11U, 0U, 1U, 0U, 10U, 11U, 10U, 0U, 255U), 
			PACKSIXTEEN(11U, 10U, 0U, 11U, 0U, 3U, 10U, 5U, 0U, 8U, 0U, 7U, 5U, 7U, 0U, 255U), 
			PACKSIXTEEN(11U, 10U, 5U, 7U, 11U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 6U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 3U, 5U, 10U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 0U, 1U, 5U, 10U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 8U, 3U, 1U, 9U, 8U, 5U, 10U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 6U, 5U, 2U, 6U, 1U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 6U, 5U, 1U, 2U, 6U, 3U, 0U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 6U, 5U, 9U, 0U, 6U, 0U, 2U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 9U, 8U, 5U, 8U, 2U, 5U, 2U, 6U, 3U, 2U, 8U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 3U, 11U, 10U, 6U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(11U, 0U, 8U, 11U, 2U, 0U, 10U, 6U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 1U, 9U, 2U, 3U, 11U, 5U, 10U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 10U, 6U, 1U, 9U, 2U, 9U, 11U, 2U, 9U, 8U, 11U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 3U, 11U, 6U, 5U, 3U, 5U, 1U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 11U, 0U, 11U, 5U, 0U, 5U, 1U, 5U, 11U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 11U, 6U, 0U, 3U, 6U, 0U, 6U, 5U, 0U, 5U, 9U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 5U, 9U, 6U, 9U, 11U, 11U, 9U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 10U, 6U, 4U, 7U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 3U, 0U, 4U, 7U, 3U, 6U, 5U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 9U, 0U, 5U, 10U, 6U, 8U, 4U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 6U, 5U, 1U, 9U, 7U, 1U, 7U, 3U, 7U, 9U, 4U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 1U, 2U, 6U, 5U, 1U, 4U, 7U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 5U, 5U, 2U, 6U, 3U, 0U, 4U, 3U, 4U, 7U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 4U, 7U, 9U, 0U, 5U, 0U, 6U, 5U, 0U, 2U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 3U, 9U, 7U, 9U, 4U, 3U, 2U, 9U, 5U, 9U, 6U, 2U, 6U, 9U, 255U), 
			PACKSIXTEEN(3U, 11U, 2U, 7U, 8U, 4U, 10U, 6U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 10U, 6U, 4U, 7U, 2U, 4U, 2U, 0U, 2U, 7U, 11U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 1U, 9U, 4U, 7U, 8U, 2U, 3U, 11U, 5U, 10U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 2U, 1U, 9U, 11U, 2U, 9U, 4U, 11U, 7U, 11U, 4U, 5U, 10U, 6U, 255U), 
			PACKSIXTEEN(8U, 4U, 7U, 3U, 11U, 5U, 3U, 5U, 1U, 5U, 11U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 1U, 11U, 5U, 11U, 6U, 1U, 0U, 11U, 7U, 11U, 4U, 0U, 4U, 11U, 255U), 
			PACKSIXTEEN(0U, 5U, 9U, 0U, 6U, 5U, 0U, 3U, 6U, 11U, 6U, 3U, 8U, 4U, 7U, 255U), 
			PACKSIXTEEN(6U, 5U, 9U, 6U, 9U, 11U, 4U, 7U, 9U, 7U, 11U, 9U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 4U, 9U, 6U, 4U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 10U, 6U, 4U, 9U, 10U, 0U, 8U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 0U, 1U, 10U, 6U, 0U, 6U, 4U, 0U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 3U, 1U, 8U, 1U, 6U, 8U, 6U, 4U, 6U, 1U, 10U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 4U, 9U, 1U, 2U, 4U, 2U, 6U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 0U, 8U, 1U, 2U, 9U, 2U, 4U, 9U, 2U, 6U, 4U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 2U, 4U, 4U, 2U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 3U, 2U, 8U, 2U, 4U, 4U, 2U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 4U, 9U, 10U, 6U, 4U, 11U, 2U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 2U, 2U, 8U, 11U, 4U, 9U, 10U, 4U, 10U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 11U, 2U, 0U, 1U, 6U, 0U, 6U, 4U, 6U, 1U, 10U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 4U, 1U, 6U, 1U, 10U, 4U, 8U, 1U, 2U, 1U, 11U, 8U, 11U, 1U, 255U), 
			PACKSIXTEEN(9U, 6U, 4U, 9U, 3U, 6U, 9U, 1U, 3U, 11U, 6U, 3U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 11U, 1U, 8U, 1U, 0U, 11U, 6U, 1U, 9U, 1U, 4U, 6U, 4U, 1U, 255U), 
			PACKSIXTEEN(3U, 11U, 6U, 3U, 6U, 0U, 0U, 6U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 4U, 8U, 11U, 6U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 10U, 6U, 7U, 8U, 10U, 8U, 9U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 7U, 3U, 0U, 10U, 7U, 0U, 9U, 10U, 6U, 7U, 10U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 6U, 7U, 1U, 10U, 7U, 1U, 7U, 8U, 1U, 8U, 0U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 6U, 7U, 10U, 7U, 1U, 1U, 7U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 6U, 1U, 6U, 8U, 1U, 8U, 9U, 8U, 6U, 7U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 6U, 9U, 2U, 9U, 1U, 6U, 7U, 9U, 0U, 9U, 3U, 7U, 3U, 9U, 255U), 
			PACKSIXTEEN(7U, 8U, 0U, 7U, 0U, 6U, 6U, 0U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 3U, 2U, 6U, 7U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 3U, 11U, 10U, 6U, 8U, 10U, 8U, 9U, 8U, 6U, 7U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 0U, 7U, 2U, 7U, 11U, 0U, 9U, 7U, 6U, 7U, 10U, 9U, 10U, 7U, 255U), 
			PACKSIXTEEN(1U, 8U, 0U, 1U, 7U, 8U, 1U, 10U, 7U, 6U, 7U, 10U, 2U, 3U, 11U, 255U), 
			PACKSIXTEEN(11U, 2U, 1U, 11U, 1U, 7U, 10U, 6U, 1U, 6U, 7U, 1U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 9U, 6U, 8U, 6U, 7U, 9U, 1U, 6U, 11U, 6U, 3U, 1U, 3U, 6U, 255U), 
			PACKSIXTEEN(0U, 9U, 1U, 11U, 6U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 8U, 0U, 7U, 0U, 6U, 3U, 11U, 0U, 11U, 6U, 0U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 11U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 6U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 0U, 8U, 11U, 7U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 1U, 9U, 11U, 7U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 1U, 9U, 8U, 3U, 1U, 11U, 7U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 1U, 2U, 6U, 11U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 10U, 3U, 0U, 8U, 6U, 11U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 9U, 0U, 2U, 10U, 9U, 6U, 11U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 11U, 7U, 2U, 10U, 3U, 10U, 8U, 3U, 10U, 9U, 8U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 2U, 3U, 6U, 2U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 0U, 8U, 7U, 6U, 0U, 6U, 2U, 0U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 7U, 6U, 2U, 3U, 7U, 0U, 1U, 9U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 6U, 2U, 1U, 8U, 6U, 1U, 9U, 8U, 8U, 7U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 7U, 6U, 10U, 1U, 7U, 1U, 3U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 7U, 6U, 1U, 7U, 10U, 1U, 8U, 7U, 1U, 0U, 8U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 3U, 7U, 0U, 7U, 10U, 0U, 10U, 9U, 6U, 10U, 7U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 6U, 10U, 7U, 10U, 8U, 8U, 10U, 9U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 8U, 4U, 11U, 8U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 6U, 11U, 3U, 0U, 6U, 0U, 4U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 6U, 11U, 8U, 4U, 6U, 9U, 0U, 1U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 4U, 6U, 9U, 6U, 3U, 9U, 3U, 1U, 11U, 3U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 8U, 4U, 6U, 11U, 8U, 2U, 10U, 1U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 10U, 3U, 0U, 11U, 0U, 6U, 11U, 0U, 4U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 11U, 8U, 4U, 6U, 11U, 0U, 2U, 9U, 2U, 10U, 9U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 9U, 3U, 10U, 3U, 2U, 9U, 4U, 3U, 11U, 3U, 6U, 4U, 6U, 3U, 255U), 
			PACKSIXTEEN(8U, 2U, 3U, 8U, 4U, 2U, 4U, 6U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 4U, 2U, 4U, 6U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 9U, 0U, 2U, 3U, 4U, 2U, 4U, 6U, 4U, 3U, 8U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 9U, 4U, 1U, 4U, 2U, 2U, 4U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 1U, 3U, 8U, 6U, 1U, 8U, 4U, 6U, 6U, 10U, 1U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 1U, 0U, 10U, 0U, 6U, 6U, 0U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 6U, 3U, 4U, 3U, 8U, 6U, 10U, 3U, 0U, 3U, 9U, 10U, 9U, 3U, 255U), 
			PACKSIXTEEN(10U, 9U, 4U, 6U, 10U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 9U, 5U, 7U, 6U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 3U, 4U, 9U, 5U, 11U, 7U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 0U, 1U, 5U, 4U, 0U, 7U, 6U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(11U, 7U, 6U, 8U, 3U, 4U, 3U, 5U, 4U, 3U, 1U, 5U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 5U, 4U, 10U, 1U, 2U, 7U, 6U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 11U, 7U, 1U, 2U, 10U, 0U, 8U, 3U, 4U, 9U, 5U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 6U, 11U, 5U, 4U, 10U, 4U, 2U, 10U, 4U, 0U, 2U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 4U, 8U, 3U, 5U, 4U, 3U, 2U, 5U, 10U, 5U, 2U, 11U, 7U, 6U, 255U), 
			PACKSIXTEEN(7U, 2U, 3U, 7U, 6U, 2U, 5U, 4U, 9U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 5U, 4U, 0U, 8U, 6U, 0U, 6U, 2U, 6U, 8U, 7U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 6U, 2U, 3U, 7U, 6U, 1U, 5U, 0U, 5U, 4U, 0U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 2U, 8U, 6U, 8U, 7U, 2U, 1U, 8U, 4U, 8U, 5U, 1U, 5U, 8U, 255U), 
			PACKSIXTEEN(9U, 5U, 4U, 10U, 1U, 6U, 1U, 7U, 6U, 1U, 3U, 7U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 6U, 10U, 1U, 7U, 6U, 1U, 0U, 7U, 8U, 7U, 0U, 9U, 5U, 4U, 255U), 
			PACKSIXTEEN(4U, 0U, 10U, 4U, 10U, 5U, 0U, 3U, 10U, 6U, 10U, 7U, 3U, 7U, 10U, 255U), 
			PACKSIXTEEN(7U, 6U, 10U, 7U, 10U, 8U, 5U, 4U, 10U, 4U, 8U, 10U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 9U, 5U, 6U, 11U, 9U, 11U, 8U, 9U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 6U, 11U, 0U, 6U, 3U, 0U, 5U, 6U, 0U, 9U, 5U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 11U, 8U, 0U, 5U, 11U, 0U, 1U, 5U, 5U, 6U, 11U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(6U, 11U, 3U, 6U, 3U, 5U, 5U, 3U, 1U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 10U, 9U, 5U, 11U, 9U, 11U, 8U, 11U, 5U, 6U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 11U, 3U, 0U, 6U, 11U, 0U, 9U, 6U, 5U, 6U, 9U, 1U, 2U, 10U, 255U), 
			PACKSIXTEEN(11U, 8U, 5U, 11U, 5U, 6U, 8U, 0U, 5U, 10U, 5U, 2U, 0U, 2U, 5U, 255U), 
			PACKSIXTEEN(6U, 11U, 3U, 6U, 3U, 5U, 2U, 10U, 3U, 10U, 5U, 3U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 8U, 9U, 5U, 2U, 8U, 5U, 6U, 2U, 3U, 8U, 2U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 5U, 6U, 9U, 6U, 0U, 0U, 6U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 5U, 8U, 1U, 8U, 0U, 5U, 6U, 8U, 3U, 8U, 2U, 6U, 2U, 8U, 255U), 
			PACKSIXTEEN(1U, 5U, 6U, 2U, 1U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 3U, 6U, 1U, 6U, 10U, 3U, 8U, 6U, 5U, 6U, 9U, 8U, 9U, 6U, 255U), 
			PACKSIXTEEN(10U, 1U, 0U, 10U, 0U, 6U, 9U, 5U, 0U, 5U, 6U, 0U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 3U, 8U, 5U, 6U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 5U, 6U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(11U, 5U, 10U, 7U, 5U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(11U, 5U, 10U, 11U, 7U, 5U, 8U, 3U, 0U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 11U, 7U, 5U, 10U, 11U, 1U, 9U, 0U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 7U, 5U, 10U, 11U, 7U, 9U, 8U, 1U, 8U, 3U, 1U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(11U, 1U, 2U, 11U, 7U, 1U, 7U, 5U, 1U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 3U, 1U, 2U, 7U, 1U, 7U, 5U, 7U, 2U, 11U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 7U, 5U, 9U, 2U, 7U, 9U, 0U, 2U, 2U, 11U, 7U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(7U, 5U, 2U, 7U, 2U, 11U, 5U, 9U, 2U, 3U, 2U, 8U, 9U, 8U, 2U, 255U), 
			PACKSIXTEEN(2U, 5U, 10U, 2U, 3U, 5U, 3U, 7U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 2U, 0U, 8U, 5U, 2U, 8U, 7U, 5U, 10U, 2U, 5U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 0U, 1U, 5U, 10U, 3U, 5U, 3U, 7U, 3U, 10U, 2U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 8U, 2U, 9U, 2U, 1U, 8U, 7U, 2U, 10U, 2U, 5U, 7U, 5U, 2U, 255U), 
			PACKSIXTEEN(1U, 3U, 5U, 3U, 7U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 7U, 0U, 7U, 1U, 1U, 7U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 0U, 3U, 9U, 3U, 5U, 5U, 3U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 8U, 7U, 5U, 9U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 8U, 4U, 5U, 10U, 8U, 10U, 11U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 0U, 4U, 5U, 11U, 0U, 5U, 10U, 11U, 11U, 3U, 0U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 1U, 9U, 8U, 4U, 10U, 8U, 10U, 11U, 10U, 4U, 5U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(10U, 11U, 4U, 10U, 4U, 5U, 11U, 3U, 4U, 9U, 4U, 1U, 3U, 1U, 4U, 255U), 
			PACKSIXTEEN(2U, 5U, 1U, 2U, 8U, 5U, 2U, 11U, 8U, 4U, 5U, 8U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 4U, 11U, 0U, 11U, 3U, 4U, 5U, 11U, 2U, 11U, 1U, 5U, 1U, 11U, 255U), 
			PACKSIXTEEN(0U, 2U, 5U, 0U, 5U, 9U, 2U, 11U, 5U, 4U, 5U, 8U, 11U, 8U, 5U, 255U), 
			PACKSIXTEEN(9U, 4U, 5U, 2U, 11U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 5U, 10U, 3U, 5U, 2U, 3U, 4U, 5U, 3U, 8U, 4U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(5U, 10U, 2U, 5U, 2U, 4U, 4U, 2U, 0U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 10U, 2U, 3U, 5U, 10U, 3U, 8U, 5U, 4U, 5U, 8U, 0U, 1U, 9U, 255U), 
			PACKSIXTEEN(5U, 10U, 2U, 5U, 2U, 4U, 1U, 9U, 2U, 9U, 4U, 2U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 4U, 5U, 8U, 5U, 3U, 3U, 5U, 1U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 4U, 5U, 1U, 0U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(8U, 4U, 5U, 8U, 5U, 3U, 9U, 0U, 5U, 0U, 3U, 5U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 4U, 5U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 11U, 7U, 4U, 9U, 11U, 9U, 10U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 8U, 3U, 4U, 9U, 7U, 9U, 11U, 7U, 9U, 10U, 11U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 10U, 11U, 1U, 11U, 4U, 1U, 4U, 0U, 7U, 4U, 11U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 1U, 4U, 3U, 4U, 8U, 1U, 10U, 4U, 7U, 4U, 11U, 10U, 11U, 4U, 255U), 
			PACKSIXTEEN(4U, 11U, 7U, 9U, 11U, 4U, 9U, 2U, 11U, 9U, 1U, 2U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 7U, 4U, 9U, 11U, 7U, 9U, 1U, 11U, 2U, 11U, 1U, 0U, 8U, 3U, 255U), 
			PACKSIXTEEN(11U, 7U, 4U, 11U, 4U, 2U, 2U, 4U, 0U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(11U, 7U, 4U, 11U, 4U, 2U, 8U, 3U, 4U, 3U, 2U, 4U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 9U, 10U, 2U, 7U, 9U, 2U, 3U, 7U, 7U, 4U, 9U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 10U, 7U, 9U, 7U, 4U, 10U, 2U, 7U, 8U, 7U, 0U, 2U, 0U, 7U, 255U), 
			PACKSIXTEEN(3U, 7U, 10U, 3U, 10U, 2U, 7U, 4U, 10U, 1U, 10U, 0U, 4U, 0U, 10U, 255U), 
			PACKSIXTEEN(1U, 10U, 2U, 8U, 7U, 4U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 9U, 1U, 4U, 1U, 7U, 7U, 1U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 9U, 1U, 4U, 1U, 7U, 0U, 8U, 1U, 8U, 7U, 1U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 0U, 3U, 7U, 4U, 3U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(4U, 8U, 7U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 10U, 8U, 10U, 11U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 0U, 9U, 3U, 9U, 11U, 11U, 9U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 1U, 10U, 0U, 10U, 8U, 8U, 10U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 1U, 10U, 11U, 3U, 10U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 2U, 11U, 1U, 11U, 9U, 9U, 11U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 0U, 9U, 3U, 9U, 11U, 1U, 2U, 9U, 2U, 11U, 9U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 2U, 11U, 8U, 0U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(3U, 2U, 11U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 3U, 8U, 2U, 8U, 10U, 10U, 8U, 9U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(9U, 10U, 2U, 0U, 9U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(2U, 3U, 8U, 2U, 8U, 10U, 0U, 1U, 8U, 1U, 10U, 8U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 10U, 2U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(1U, 3U, 8U, 9U, 1U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 9U, 1U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(0U, 3U, 8U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U), 
			PACKSIXTEEN(255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U, 255U)
		}

	};
	tables = new UniformBufferObject<LookupTables>("LookupTablesMC");
	//tables->Initialize();
	tables->Update(&table);
}