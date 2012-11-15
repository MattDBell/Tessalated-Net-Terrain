#version 420

layout(binding = 1) uniform sampler3D samp;

layout( std140 ) uniform LookupTablesMC {
	uvec4 edgeTable[64];
	uvec4 triTable[256];
};

layout ( std140 ) uniform CamMats{
	mat4 view;
	mat4 proj;
};

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

in VertexData {
	vec4 uvwCoords;
	vec4 right;
	vec4 up;
	vec4 forward;
} inData[];

out PixelData{
	vec3 normal;
	vec4 worldPos;
	vec4 shapeColor;
};


void main(void){

	//unsigned int masks[4] = {0x000000ff, 0x0000ff00,0x00ff0000,  0xff000000};
	//unsigned int shifts[4] = { 0, 8,16,24 };
	unsigned int masks[4] = {0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff};
	unsigned int shifts[4] = {24, 16, 8, 0 };
	unsigned int edgeVers[24] = { 0, 1, 1, 2, 2, 3, 3, 0,
								  4, 5, 5, 6, 6, 7, 7, 4,
								  4, 0, 5, 1, 6, 2, 7, 3 };


	vec4 corners[8] = {gl_in[0].gl_Position, //bottom left back
	gl_in[0].gl_Position + inData[0].right,
	gl_in[0].gl_Position + inData[0].forward + inData[0].right,
	gl_in[0].gl_Position + inData[0].forward,
	gl_in[0].gl_Position + inData[0].up,
	gl_in[0].gl_Position + inData[0].up + inData[0].right,
	gl_in[0].gl_Position + inData[0].up + inData[0].forward + inData[0].right,
	gl_in[0].gl_Position + inData[0].up + inData[0].forward};
	
	ivec4 diffs[8] = {	ivec4(0, 0, 0, 0), ivec4(1, 0, 0, 0), ivec4(1, 0, 1, 0), ivec4(0, 0, 1, 0),
						ivec4(0, 1, 0, 0), ivec4(1, 1, 0, 0), ivec4(1, 1, 1, 0), ivec4(0, 1, 1, 0)};

	float densities[8];
	unsigned int gridIndex = 0;

	vec4 verts[12] = {	vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), 
	vec4( 0, 0, 0, 0),	vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), 
	vec4( 0, 0, 0, 0),	vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0)};

	for(int i = 0; i < 8; ++i){
		ivec4 loc = ivec4(inData[0].uvwCoords)+ diffs[i];
		densities[i] = texelFetch(samp, loc.xyz, 0).r;
		gridIndex |= (densities[i] < 0) ? (i << i) : 0;
	}

	uint edge = edgeTable[gridIndex >> 2U][gridIndex & 3U];
	for(int i = 0; i < 12; ++i){
		if((edge & (1 << i)) != 0){
			unsigned int ver[2] = {edgeVers[i*2], edgeVers[i*2 + 1]};
			verts[i] = corners[ver[0]] + (-densities[ver[0]] * (corners[ver[1]] - corners[ver[0]])/ (densities[ver[1]] - densities[ver[0]]) ) ;
		}
	}

	//Put verts into tristream
	int vertCount = 0;
	vec4 tris[15]= {vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),
				 	vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0),vec4(0, 0, 0, 0) };

	//This breaks glsl
	uint Vees[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for(int pack = 0; pack < 4; ++pack){
		for(int item = 0; item < 4; ++item){
			Vees[pack * 4 + item]= (triTable[gridIndex][pack] & masks[item]) >> shifts[item];
			//uint v  = (triTable[gridIndex][pack] & masks[item]) >> shifts[item];
			//if(v < 255){
			//	tris[vertCount] = verts[v];
			//	vertCount += 1;
			//}
		}
	}

	for(int i = 0; i < 15 && Vees[i] != 255 ; ++i){
		if(Vees[i] < 255){
			tris[i] = verts[Vees[i]];
			vertCount += 1;
		}
	}

	float c = float(vertCount) / 15.0;
	shapeColor = vec4(c, c, c, 1);
	for(int y = 0; y < vertCount; y += 3){
		vec3 norm = cross(tris[y + 1].xyz - tris[y].xyz, tris[y+2].xyz - tris[y].xyz);
		gl_Position = proj * view * tris[y];
		normal = norm;
		worldPos = tris[y];
		EmitVertex();
		gl_Position = proj * view * tris[y+1];
		normal = norm;
		worldPos = tris[y + 1];
		EmitVertex();
		gl_Position = proj * view * tris[y+2];
		normal = norm;
		worldPos = tris[y + 2];
		EmitVertex();
		EndPrimitive();
	}

}