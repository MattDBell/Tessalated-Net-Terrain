layout(binding = 0) uniform sampler3D samp;

layout( std140 ) uniform MCIndices {
	ivec4 edgeTable[64];
	ivec4 triTable[256];
};