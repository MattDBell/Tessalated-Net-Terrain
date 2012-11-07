layout(binding = 0) uniform sampler3D samp;

layout( std140 ) uniform MCIndices {
	ivec4 edgeTable[64];
	ivec4 triTable[256];
};

layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

in VertexData {
	vec4 uvwCoords;
	vec4 right;
	vec4 up;
	vec4 forward;
} inData[];



void main(void){
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
	float4 tris[15] = {float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),
	float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),
	float4( 0, 0, 0, 0),float4( 0, 0, 0, 0),float4( 0, 0, 0, 0)};
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
		PSInput goingOut1 = (PSInput)0;
		PSInput goingOut2 = (PSInput)0;
		PSInput goingOut3 = (PSInput)0;
		goingOut1.pos = tris[outputs];
		goingOut2.pos = tris[outputs + 1];
		goingOut3.pos = tris[outputs + 2];
		triStream.Append(goingOut1);
		triStream.Append(goingOut2);
		triStream.Append(goingOut3);
		triStream.RestartStrip();
		outputs += 3;
	}

}