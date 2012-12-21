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

uniform mat4 model;
uniform float time;
uniform int state;
out PixelData{
	vec3 normal;
	noperspective vec4 objectPos;
	vec4 worldPos;
};

float GetValueAt(ivec3 pos)
{
	if(state == 5)
		return -1.0  + (pos.x % 4);  
	else
		return texelFetch(samp, pos , 0).r; 
}

void main(void)
{
	vec4 diffs[8] = {	vec4(0, 0, 0, 0), vec4(1, 0, 0, 0), vec4(1, 0, 1, 0), vec4(0, 0, 1, 0),
						vec4(0, 1, 0, 0), vec4(1, 1, 0, 0), vec4(1, 1, 1, 0), vec4(0, 1, 1, 0)};
	unsigned int masks[4] = {0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff};
	unsigned int shifts[4] = {24, 16, 8, 0 };
	float isoLevel = 0;
	//=====================Find Grid Index ======================
	
	uint gridIndex = 0;
	float[8] value;
	value[0] =	GetValueAt( ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[0].xyz));	// texelFetch(samp, ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[0].xyz), 0).r ;
	value[1] =	GetValueAt( ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[1].xyz));	// texelFetch(samp, ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[1].xyz), 0).r ;
	value[2] =	GetValueAt( ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[2].xyz));	// texelFetch(samp, ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[2].xyz), 0).r ;
	value[3] =	GetValueAt( ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[3].xyz));	// texelFetch(samp, ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[3].xyz), 0).r ;
	value[4] =	GetValueAt( ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[4].xyz));	// texelFetch(samp, ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[4].xyz), 0).r ;
	value[5] =	GetValueAt( ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[5].xyz));	// texelFetch(samp, ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[5].xyz), 0).r ;
	value[6] =	GetValueAt( ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[6].xyz));	// texelFetch(samp, ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[6].xyz), 0).r ;
	value[7] =	GetValueAt( ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[7].xyz));	// texelFetch(samp, ivec3( ( gl_in[0].gl_Position.xyz ) + diffs[7].xyz), 0).r ;
	
	for(int i = 0; i < 8; ++i)
	{	
		if(value[i] < isoLevel) gridIndex |= (1 << i);
	}

	uint edge = edgeTable[gridIndex / 4][gridIndex  % 4];
	vec4 verts[12];
	uint ver2s[12] = {1, 2, 3, 0, 5, 6, 7, 4, 0, 1, 2, 3};
	for(uint i = 0; i < 12; ++i)
	{
		if(((edge / uint(pow(2, i)))  % 2) == 1U)
		{
			uint ver1;
			if(i < 8){
				ver1 = i;
			} else {
				ver1 = i -4;
			}
			uint ver2 = ver2s[i];
			vec4 vert1= gl_in[0].gl_Position + diffs[ver1];
			vec4 vert2= gl_in[0].gl_Position + diffs[ver2];
			verts[i] = vert1 + (vert1 - vert2) * (isoLevel- value[ver1]) / (value[ver1] - value[ver2]);
			//verts[i] = vert1 + (-value[ver1] * (vert2 - vert1)/ (value[ver2] - value[ver1]) ) ;
			
		}
		else
		{
			verts[i] = vec4(0, 0, 0, 0);
		}
	}
	int tris[15];
	for(int i = 0; i < 15; i += 3)
	{
		tris[i] = int((triTable[gridIndex][i/4] & masks[i%4]) >> shifts[i%4]);
		if(tris[i] < 12)
		{
			tris[i + 1] = int((triTable[gridIndex][(i + 1) /4] & masks[(i + 1) %4]) >> shifts[(i + 1) %4]);
			tris[i + 2] = int((triTable[gridIndex][(i + 2) /4] & masks[(i + 2) %4]) >> shifts[(i + 2) %4]);
		}
		else
		{
			tris[i] = 0;
			tris[i + 1] = 0;
			tris[i + 2] = 0;
			break; //This hopefully works, if not then the three lines above should help.
		}
		

		vec4 wPoses[3];
		wPoses[0] = model * verts[tris[i]];
		wPoses[2] = model * verts[tris[i +1]];
		wPoses[1] = model * verts[tris[i +2]];

		normal = normalize( cross ( ( wPoses[1] - wPoses[0]).xyz, (wPoses[2] - wPoses[0]).xyz));

		objectPos = verts[tris[i]];
		worldPos = wPoses[0];
		gl_Position = proj * view * worldPos;
		EmitVertex();

		objectPos = verts[tris[i + 2]];
		worldPos = wPoses[1];
		gl_Position = proj * view * worldPos;
		EmitVertex();

		objectPos = verts[tris[i + 1]];
		worldPos = wPoses[2];
		gl_Position = proj * view * worldPos;
		EmitVertex();
		
		EndPrimitive();
	
	}

}

