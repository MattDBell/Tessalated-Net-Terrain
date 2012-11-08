#version 420

layout(binding = 0) uniform sampler3D samp;

layout( std140 ) uniform LookupTablesMC {
	uvec4 edgeTable[64];
	uvec4 triTable[256];
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

	unsigned int masks[4] = {0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000};
	unsigned int shifts[4] = {0, 8, 16, 24 };
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
	vec4 tris[15] = {	vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),
	vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),
	vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0),vec4( 0, 0, 0, 0)};
	for(int pack = 0; pack < 4; ++pack){
		for(int item = 0; item < 4; ++item){
			unsigned int v = (triTable[gridIndex][pack] & masks[item]) >> shifts[item];
			if( v == 255)
				break;
			tris[vertCount] = verts[v];
			vertCount += 1;
		}
	}
	int output = 0;
	for(int y = 0; y < 15; y += 3){
		gl_Position = tris[output];
		EmitVertex();
		gl_Position = tris[output+1];
		EmitVertex();
		gl_Position = tris[output+2];
		EmitVertex();
		EndPrimitive();
		output += 3;
	}

}