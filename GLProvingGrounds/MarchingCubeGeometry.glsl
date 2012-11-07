layout(binding = 0) uniform sampler3D samp;

layout( std140 ) uniform LookupTablesMC {
	ivec4 edgeTable[64];
	ivec4 triTable[256];
};

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

in VertexData {
	vec4 uvwCoords;
	vec4 right;
	vec4 up;
	vec4 forward;
} inData[];



void main(void){
	int3 topRightForwardDiff = int3( 1, 1, 1);
	int3 bottomLeftBackDiff = int3( 0, 0, 0);
	unsigned int masks[4] = {0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000};
	unsigned int shifts[4] = {0, 8, 16, 24 };
	unsigned int edgeVers[12][2] = { { 0, 1}, {1, 2}, {2, 3}, {3, 0},
	{4, 5}, {5, 6}, {6, 7}, {7, 4},
	{4, 0}, {5, 1}, {6, 2}, {7, 3} };
	int3 diffs[8] = {
	int3(bottomLeftBackDiff.xyz),
	int3(topRightForwardDiff.x, bottomLeftBackDiff.yz),
	int3(topRightForwardDiff.x, bottomLeftBackDiff.y, topRightForwardDiff.z),
	int3(bottomLeftBackDiff.xy, topRightForwardDiff.z),
	int3(bottomLeftBackDiff.x, topRightForwardDiff.y, bottomLeftBackDiff.z),
	int3(topRightForwardDiff.xy,  bottomLeftBackDiff.z),
	int3(topRightForwardDiff.xyz),
	int3(bottomLeftBackDiff.x, topRightForwardDiff.yz)
	 };


	vec4 corners[8] = {gl_in[0].gl_position, //bottom left back
	gl_in[0].gl_position + inData[0].right,
	gl_in[0].gl_position + inData[0].forward + inData[0].right,
	gl_in[0].gl_position + inData[0].forward,
	gl_in[0].gl_position + inData[0].top,
	gl_in[0].gl_position + inData[0].top + inData[0].right,
	gl_in[0].gl_position + inData[0].top + inData[0].forward + inData[0].right,
	gl_in[0].gl_position + inData[0].top + inData[0].forward};
	
	ivec4 diffs[8] = {	ivec4(0, 0, 0, 0), ivec4(1, 0, 0, 0), ivec4(1, 0, 1, 0), ivec4(0, 0, 1, 0),
						ivec4(0, 1, 0, 0), ivec4(1, 1, 0, 0), ivec4(1, 1, 1, 0), ivec4(0, 1, 1, 0)};

	float densities[8];
	unsigned int gridIndex = 0;

	vec4 verts[12] = {	vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), 
	vec4( 0, 0, 0, 0),	vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0), 
	vec4( 0, 0, 0, 0),	vec4( 0, 0, 0, 0), vec4( 0, 0, 0, 0)};

	for(int i = 0; i < 8; ++i){
		ivec4 loc = ivec4(inData[0].uvwCoords)+ diffs[i];
		densities[i] = map.Load(loc);
		gridIndex |= (densities[i] < 0) ? (i << i) : 0;
	}

	unsigned int edge = edgeTable[gridIndex >> 2][gridIndex & 3];
	for(int i = 0; i < 12; ++i){
		if((edge & (1 << i)) != 0){
			unsigned int ver[2] = {edgeVers[i][0], edgeVers[i][1]};
			verts[i] = corners[ver[0]] + (-densities[ver[0]] * (corners[ver[1]] - corners[ver[0]])/ ((float)densities[ver[1]] - (float)densities[ver[0]]) ) ;
		}
	}

	//Put verts into tristream
	int vertCount = 0;
	vec4 tris[15] = {	vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),
	vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),
	vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0)};
	for(int pack = 0; pack < 4; ++pack){
		for(int item = 0; item < 4; ++item){
			int v = (triTable[gridindex][pack] & masks[item]) >> shifts[item];
			if( v == 255)
				break;
			tris[vertCount] = verts[v];
			vertCount += 1;
		}
	}
	int outputs = 0;
	for(int y = 0; y < 15; y += 3){
		gl_PerVertex.glPosition = tris[output];
		EmitVertex();
		gl_PerVertex.glPosition = tris[output];
		EmitVertex();
		gl_PerVertex.glPosition = tris[output];
		EmitVertex();
		EndPrimitive();
		outputs += 3;
	}

}