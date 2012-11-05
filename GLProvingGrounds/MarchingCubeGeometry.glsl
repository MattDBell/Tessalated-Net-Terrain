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
		
	}

}